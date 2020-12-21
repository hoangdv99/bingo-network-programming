#include "gameplay.h"

void initGame(ROOM *room)
{
    printf("Boards generating!\n");
    int n = 0;
    srand(time(NULL));
    for (n = 0; n < room->playerAmount; n++)
    {
        int a[SIZE * SIZE], i, j, x, y;
        x = 0;
        y = 0;
        int temp;

        for (i = 0; i < SIZE * SIZE; i++)
        {
            temp = rand() % (SIZE * SIZE) + 1;
            for (j = 0; j < i; j++)
            {
                if (temp == a[j])
                {
                    break;
                }
            }
            if (i == j)
            {
                a[i] = temp;
            }
            else
            {
                i--;
            }
        }
        //printf("\n  %s  \n", room->player[n]->username);
        for (i = 0; i < SIZE * SIZE; i++)
        {
            room->player[n]->board[x][y] = a[i];
            if ((i + 1) % 5 == 0)
            {
                x++;
                y = 0;
                //printf("%d", x);
            }
            else
            {
                y++;
            }
        }
    }
    printf("Board generated!\n");
}

int pickNumber(ROOM *room, int number)
{ //return 0=>pick fail, return 1=>pick ok
    int picked = 0;
    for (int i = 0; i < room->numberPickedAmount; i++)
    {
        if (number == room->pickedNumbers[i])
        {
            return 0;
        }
    }
    room->pickedNumbers[room->numberPickedAmount] = number;
    room->numberPickedAmount++;

    for (int i = 0; i < room->playerAmount; i++)
    {
        for (int j = 0; j < SIZE; j++)
        {
            for (int k = 0; k < SIZE; k++)
            {
                if (room->player[i]->board[j][k] == number)
                {
                    room->player[i]->board[j][k] = 0;
                    picked++;
                }
            }
        }
    }
    return picked;
}

int checkBingo(ROOM *room, USER *user) //return 1 if bingo
{
    int j, k;
    int sum_row, sum_col;
    int cross_1 = 0;
    int cross_2 = 0;

    for (j = 0; j < SIZE; j++)
    {
        sum_row = 0;
        sum_col = 0;
        for (k = 0; k < SIZE; k++)
        {
            //printf("%d\t", room->player[i]->board[j][k]);
            sum_row += user->board[j][k];
            sum_col += user->board[k][j];
            if (j == k)
            {
                cross_1 += user->board[j][k];
            }
            if ((j + k) == 4)
            {
                cross_2 += user->board[j][k];
            }
        }
        if ((sum_col == 0) || (sum_row == 0) || (cross_1 == 0) || (cross_2 == 0))
        {
            printf("Player %s BINGO\n", user->username);
            return 1;
        }
    }
    return 0;
}

void roomThreadFunc(void *arg)
{
    THREAD_DATA *threadData = (THREAD_DATA *)arg;
    initGame(threadData->room);
    int playingClientfd, remain_time = 15, bingo = 0, turn = 0, max_clients = 30, i, activity, sd, client_socket[30];
    struct timeval timeout;
    time_t now, end_time;
    Request *req = (Request *)malloc(sizeof(Request));
    Response *res = (Response *)malloc(sizeof(Response));
    USER *winner;
    while (bingo != 1)
    {
        timeout.tv_sec = remain_time;
        timeout.tv_usec = 0;
        now = time(NULL);
        end_time = now + remain_time;
        playingClientfd = threadData->room->player[turn]->clientfd;

        //clear the socket set
        FD_ZERO(&threadData->readfds);
        threadData->max_sd = threadData->master_socket;
        //add child sockets to set
        for (int i = 0; i < max_clients; i++)
        {
            //socket descriptor
            sd = client_socket[i];

            //if valid socket descriptor then add to read list
            if (sd > 0)
                FD_SET(sd, &threadData->readfds);

            //highest file descriptor number, need it for the select function
            if (sd > threadData->max_sd)
                threadData->max_sd = sd;
        }
        for (int i = 0; i < threadData->room->playerAmount; i++)
        {
            sd = threadData->room->player[i]->clientfd;
            if (i == turn)
            {
                res->code = YOUR_TURN;
                setMessageResponse(res);
                sendRes(sd, res, sizeof(Response), 0);
            }
            else
            {
                res->code = OTHER_PLAYER_TURN;
                strcpy(res->data, threadData->room->player[turn]->username);
                setMessageResponse(res);
                sendRes(sd, res, sizeof(Response), 0);
            }
        }
        //wait for an activity on one of the sockets
        //so wait indefinitely

        activity = select(threadData->max_sd + 1, &threadData->readfds, NULL, NULL, &timeout);
        if (activity == 0)
        {
            if (turn == threadData->room->playerAmount - 1)
            {
                turn = 0;
            }
            else
            {
                turn++;
            }
            printf(" timed out.  Bravo[%d] turn\n", threadData->room->player[turn]->clientfd);
            remain_time = 15;
            continue;
        }

        //else its some IO operation on some other socket
        for (int i = 0; i < max_clients; i++)
        {
            sd = client_socket[i];
            int j = 0, close_fd = 0;
            if (FD_ISSET(sd, &threadData->readfds))
            {
                int valread = recv(sd, req, sizeof(Request), 0);
                if (valread == 0 || req->code == EXIT_GAME)
                {
                    //Somebody disconnected , get his details and print
                    //code
                }
                else if (req->code == BINGO)
                {
                    int check;
                    for (int i = 0; i < threadData->room->playerAmount; i++)
                    {
                        if (threadData->room->player[i]->clientfd == sd)
                        {
                            check = i;
                        }
                    }
                    int result = checkBingo(threadData->room, threadData->room->player[check]);
                    if ((result == 1) && (bingo == 0))
                    {
                        res->code = BINGO_REAL;
                        setMessageResponse(res);
                        sendRes(sd, res, sizeof(Response), 0);
                        bingo = 1;
                        winner = threadData->room->player[check];
                    }
                    else
                    {
                        res->code = BINGO_FAKE;
                        setMessageResponse(res);
                        sendRes(sd, res, sizeof(Response), 0);
                        if ((end_time - time(NULL)) != 0)
                        {
                            remain_time = end_time - time(NULL);
                            printf("Bravo[%d] turn %d s remain\n", playingClientfd, remain_time);
                        }
                    }
                }
                else if (req->code == PICK)
                {
                    if (sd = playingClientfd)
                    {
                        int pickedNumber = atoi(req->message);
                        printf("Playerfd[%d]: - %d\n", sd, pickedNumber);
                        if (pickNumber(threadData->room, pickedNumber) == 0)
                        {
                            res->code = PICK_FAIL;
                            setMessageResponse(res);
                            sendRes(sd, res, sizeof(Response), 0);
                        }
                        else
                        {
                            res->code = PICK_SUCCESS;
                            sprintf(res->data, "%d", pickedNumber);
                            setMessageResponse(res);
                            remain_time = 15;

                            for (int i = 0; i < threadData->room->playerAmount; i++)
                            {
                                sendRes(threadData->room->player[i]->clientfd, res, sizeof(Response), 0);
                            }

                            if (turn == threadData->room->playerAmount - 1)
                            {
                                turn = 0;
                            }
                            else
                            {
                                turn++;
                            }
                        }
                    }
                    else
					{
                        res->code = OTHER_PLAYER_TURN;
                        setMessageResponse(res);
                        sendRes(sd, res, sizeof(Response), 0);
						if ((end_time - time(NULL)) != 0)
						{
							remain_time = end_time - time(NULL);
							printf("Bravo[%d] turn %d s remain\n", playingClientfd, remain_time);
						}
						else
						{
							if (turn == threadData->room->playerAmount - 1)
							{
								turn = 0;
							}
							else
							{
								turn++;
							}
							remain_time = 15;
						}
					}
                }
            }
        }
    }
    
    for (int i = 0; i < threadData->room->playerAmount; i++)
    {
        if(threadData->room->player[i] == winner){
            res->code = YOU_WIN;
            setMessageResponse(res);
            sendRes(winner->clientfd, res, sizeof(Response), 0);
        }else{
            res->code = OTHER_PLAYER_WIN;
            strcpy(res->data, winner->username);
            setMessageResponse(res);
            sendRes(threadData->room->player[i]->clientfd, res, sizeof(Response), 0);
        }
    }
    
}