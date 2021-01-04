#include "gameplay.h"
#define REMAIN_TIME 30
extern fd_set masterfds;

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
    for (int i = 0; i < SIZE*SIZE; i++) {
        room->pickedNumbers[i] = 0;
    }
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
    for (j = 0; j < SIZE; j++) {
        for (k = 0; k < SIZE; k++) {
            printf("%d\t", user->board[j][k]);
        }
        printf("\n");
    }
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
        if ((sum_col == 0) || (sum_row == 0))
        {
            printf("Player %s BINGO\n", user->username);
            return 1;
        }
    }
    if ( (cross_1 == 0) || (cross_2 == 0) ) {
        printf("Player %s BINGO\n", user->username);
        return 1;
    }
    return 0;
}

void sendBoardData(ROOM *room, Request *req, Response *res)
{
    char boardString[100];
    char num[5];
    
    res->code = BOARD_DATA_GENERATED;
    for (int i = 0; i < room->playerAmount; i++)
    {
        strcpy(boardString, "");
        for (int m = 0; m < SIZE; m++)
        {
            for (int n = 0; n < SIZE; n++)
            {
                sprintf(num, "%d", room->player[i]->board[m][n]);
                strcat(boardString, num);
                strcat(boardString, "-");
            }
        }
        boardString[strlen(boardString) - 1] = '\0';
        strcpy(res->data, boardString);
        setMessageResponse(res);
        sendRes(room->player[i]->clientfd, res, sizeof(Response), 0);
    }
}

void *roomThreadFunc(void *arg)
{
    THREAD_DATA *threadData = (THREAD_DATA *)arg;
    Request *req = (Request *)malloc(sizeof(Request));
    Response *res = (Response *)malloc(sizeof(Response));
    ROOM *room = findRoomByClientfd(threadData->clientfd);
    initGame(room);
    //gui bang cho tung player voi opcode BOARD_DATA_GENERATED
    sendBoardData(room, req, res);

    int playingClientfd, remain_time = REMAIN_TIME, bingo = 0, turn = 0, activity, sd, max_sd;
    struct timeval timeout;
    time_t now, end_time;

    USER *winner;
    fd_set t_readfds;
    FD_SET(threadData->master_socket, &t_readfds);

    while (bingo != 1)
    {
        FD_ZERO(&t_readfds);
        for (int i = 0; i < room->playerAmount; i++)
        {
            FD_SET(room->player[i]->clientfd, &t_readfds);
            if (room->player[i]->clientfd > max_sd)
                max_sd = room->player[i]->clientfd;
        }
        timeout.tv_sec = remain_time;
        timeout.tv_usec = 0;
        now = time(NULL);
        end_time = now + remain_time;
        playingClientfd = room->player[turn]->clientfd;
        for (int i = 0; i < room->playerAmount; i++)
        {
            sd = room->player[i]->clientfd;
            if (i == turn)
            {
                res->code = YOUR_TURN;
                setMessageResponse(res);
                sendRes(sd, res, sizeof(Response), 0);
            }
            else
            {
                res->code = OTHER_PLAYER_TURN;
                strcpy(res->data, room->player[turn]->username);
                setMessageResponse(res);
                sendRes(sd, res, sizeof(Response), 0);
            }
        }
        //wait for an activity on one of the sockets
        //so wait indefinitely

        activity = select(max_sd + 1, &t_readfds, NULL, NULL, &timeout);
        if ((activity < 0) && (errno != EINTR))
        {
            printf("select error");
        }
        else if (activity == 0) //time out
        {
            char leftPlayerUsername[50];
            strcpy(leftPlayerUsername, room->player[turn]->username);
            res->code = DISCONNECTED;
            setMessageResponse(res);
            sendRes(room->player[turn]->clientfd, res, sizeof(Response), 0);
            outRoom(room->player[turn]->clientfd, req, res);
            FD_CLR(room->player[turn]->clientfd, &t_readfds);
            //detelePlayerFromRoom(room, room->player[turn]);
            deleteUserByUsername(leftPlayerUsername);
            room->host = room->player[0];
            res->code = SOMEONE_LEFT_GAME;
            strcpy(res->data, leftPlayerUsername);
            setMessageResponse(res);

            for (int i = 0; i < room->playerAmount; i++)
            {
                sendRes(room->player[i]->clientfd, res, sizeof(Response), 0);
            }

            if (room->playerAmount == 1)
            {
                res->code = ALL_PLAYERS_LEFT_GAME;
                setMessageResponse(res);
                for (int i = 0; i < room->playerAmount; i++)
                {
                    sendRes(room->player[i]->clientfd, res, sizeof(Response), 0);
                    FD_CLR(room->player[i]->clientfd, &t_readfds);
                    FD_SET(room->player[i]->clientfd, &masterfds);
                }
                return (void *)0;
            }
            if (turn == room->playerAmount)
                turn = 0;
            printf(" timed out.  Bravo[%d] turn\n", room->player[turn]->clientfd);
            remain_time = REMAIN_TIME;
            continue;
        }
        else
        {
            //else its some IO operation on some other socket
            for (int i = 0; i < room->playerAmount; i++)
            {
                sd = room->player[i]->clientfd;
                if (FD_ISSET(sd, &t_readfds))
                {
                    printf("Receive data in socket %d\n", sd);
                    int valread = recv(sd, req, sizeof(Request), 0);
                    printf("%d\n", req->code);
                    if (valread == 0 || req->code == CLOSE || req->code == QUIT)
                    {
                        //Somebody disconnected , get his details and print
                        char leftPlayerUsername[50];
                        strcpy(leftPlayerUsername, room->player[i]->username);
                        outRoom(room->player[i]->clientfd, req, res);
                        FD_CLR(room->player[i]->clientfd, &t_readfds);
                        deleteUserByUsername(room->player[i]->username);

                        res->code = SOMEONE_LEFT_GAME;
                        strcpy(res->data, leftPlayerUsername);
                        setMessageResponse(res);

                        for (int i = 0; i < room->playerAmount; i++)
                        {
                            sendRes(room->player[i]->clientfd, res, sizeof(Response), 0);
                        }

                        if (room->playerAmount == 1)
                        {
                            res->code = ALL_PLAYERS_LEFT_GAME;
                            setMessageResponse(res);
                            for (int i = 0; i < room->playerAmount; i++)
                            {
                                sendRes(room->player[i]->clientfd, res, sizeof(Response), 0);
                                FD_CLR(room->player[i]->clientfd, &t_readfds);
                                FD_SET(room->player[i]->clientfd, &masterfds);
                            }
                            return (void *)0;
                        }
                        if(i == turn){
                            if (turn == room->playerAmount)
                                turn = 0;
                        }
                        printf(" timed out.  Bravo[%d] turn\n", room->player[turn]->clientfd);
                        remain_time = REMAIN_TIME;
                        continue;
                    }
                    else if (req->code == BINGO)
                    {
                        int check;
                        for (int i = 0; i < room->playerAmount; i++)
                        {
                            if (room->player[i]->clientfd == sd)
                            {
                                check = i;
                            }
                        }
                        int result = checkBingo(room, room->player[check]);
                        if ((result == 1) && (bingo == 0))
                        {
                            res->code = BINGO_REAL;
                            setMessageResponse(res);
                            sendRes(sd, res, sizeof(Response), 0);
                            bingo = 1;
                            winner = room->player[check];
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
                        if (sd == playingClientfd)
                        {
                            int pickedNumber = atoi(req->message);
                            if (pickNumber(room, pickedNumber) == 0)
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
                                remain_time = REMAIN_TIME;

                                for (int i = 0; i < room->playerAmount; i++)
                                {
                                    sendRes(room->player[i]->clientfd, res, sizeof(Response), 0);
                                }

                                if (turn == room->playerAmount - 1)
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
                            strcpy(res->data, room->player[turn]->username);
                            setMessageResponse(res);
                            sendRes(sd, res, sizeof(Response), 0);
                            if ((end_time - time(NULL)) != 0)
                            {
                                remain_time = end_time - time(NULL);
                                printf("Bravo[%d] turn %d s remain\n", playingClientfd, remain_time);
                            }
                            else
                            {
                                if (turn == room->playerAmount - 1)
                                {
                                    turn = 0;
                                }
                                else
                                {
                                    turn++;
                                }
                                remain_time = REMAIN_TIME;
                            }
                        }
                    }
                    else if (req->code == TEST)
                    {
                        bingo = 1;
                        winner = room->player[i];
                    }

                    else
                    {
                        printf("error!!!\n");
                    }
                }
            }
        }
    }

    for (int i = 0; i < room->playerAmount; i++)
    {
        if (room->player[i] == winner)
        {
            res->code = YOU_WIN;
            setMessageResponse(res);
            sendRes(winner->clientfd, res, sizeof(Response), 0);
        }
        else
        {
            res->code = OTHER_PLAYER_WIN;
            strcpy(res->data, winner->username);
            setMessageResponse(res);
            sendRes(room->player[i]->clientfd, res, sizeof(Response), 0);
        }
        room->status = NOTSTARTED;
        FD_CLR(room->player[i]->clientfd, &t_readfds);
        FD_SET(room->player[i]->clientfd, &masterfds);
    }
    return (void *)0;
}