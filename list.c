#include "list.h"
// Function --------------------------------------=======-----------------------------------
// USER FUNCTION
void printUser(USER *acc) 
{
    printf("%s-%d || %d\n------------------------------------\n",
           acc->username, acc->clientfd, acc->status);
    /*
    for(int i = 0; i < SIZE; i++) {
        for(int j = 0; j < SIZE; j++) {
            printf("%3d\t", acc.board[i][j] );
        }
        printf("\n");
    }
    */
    printf("------------------------------------\n");
}

USER* insertUser(char *username, int clientfd)
{
    USER* user = (USER *)malloc(sizeof(USER));
    strcpy(user->username, username);
    user->clientfd = clientfd;
    user->status = LOBBY;
    for (int i = 0; i < SIZE; i++)
    {
        for (int j = 0; j < SIZE; j++)
        {
            user->board[i][j] = 0;
        }
    }
    user->next = userListHead;
    userListHead = user;
    return user;
}

void printListUser()
{
    USER *ptr = userListHead;
    while (ptr != NULL)
    {
        printf("%s %d", ptr->username, ptr->clientfd);
        ptr = ptr->next;
    }
}

USER *findUser(char *username)
{

    USER *curr = userListHead;
    if (userListHead == NULL)
        return NULL;
    while (strcmp(curr->username, username) != 0)
    {
        if (curr->next == NULL)
            return NULL;
        else
        {
            curr = curr->next;
        }
    }
    return curr;
}

USER *deleteUser(char *username)
{
    USER *cur = userListHead;
    USER *prev = NULL;
    if (userListHead == NULL)
        return NULL;
    while (strcmp(cur->username, username) != 0)
    {
        if (cur->next == NULL)
            return NULL;
        else
        {
            prev = cur;
            cur = cur->next;
        }
    }
    if (cur == userListHead)
    {
        userListHead = userListHead->next;
    }
    else
    {
        prev->next = cur->next;
    }
    return cur;
}

// ROOM FUNCTION

void insertRoom(int id, USER *host)
{
    ROOM *newRoom = (ROOM *)malloc(sizeof(ROOM));
    newRoom->id = id;
    newRoom->host = host;
    newRoom->status = NOTSTARTED;
    newRoom->playerAmount = 0;
    newRoom->next = roomListHead;
    roomListHead = newRoom;
}

ROOM *findRoom(int id)
{
    ROOM *curr = roomListHead;
    if (curr == NULL)
        return NULL;
    while (curr->id != id)
    {
        if (curr->next == NULL)
            return NULL;
        else
        {
            curr = curr->next;
        }
    }
    return curr;
}

int insertPlayer(int id, USER *player)
{
    ROOM *room = findRoom(id);

    if (room != NULL && room->playerAmount < ROOM_MAX)
    {
        player->status = INROOM;
        room->player[room->playerAmount] = player;
        room->playerAmount++;
        return 1;
    }
    else
    {
        printf("Room is full!\n");
        return 0;
    }
}

int quickJoin(USER *player)
{
    ROOM *room = roomListHead;
    while (room != NULL)
    {
        if (room->playerAmount < ROOM_MAX)
        {
            insertPlayer(room->id, player);
            return room->id;
        }
        room = room->next;
    }
    printf("All room are full!\n");
    return -1;
}


// void printroomplayerboard(listroomptr roomPtr, char *name)
// {
//     if (roomPtr == NULL)
//     {
//         printf("Room has not created!\n");
//         return;
//     }
//     printf("Player %s --\n-------\n", name);
//     for (int i = 0; i < roomPtr->playernumber; i++)
//     {
//         if (strcmp(roomPtr->player[i].username, name) == 0)
//         {
//             for (int k = 0; k < SIZE; k++)
//             {
//                 for (int j = 0; j < SIZE; j++)
//                 {
//                     printf("%3d\t", roomPtr->player[i].board[k][j]);
//                 }
//                 printf("\n");
//             }
//         }
//     }
//     printf("\n");
// }

// PLAY FUNCTION

void playgame(int id)
{
    ROOM *room = findRoom(id);
    if (room == NULL)
    {
        printf("Room not exit");
    }
    else
    {
        int i = 0;
        srand(time(0));
        for (i = 0; i < room->playerAmount; i++)
        {

            for (int j = 0; j < SIZE; j++)
            {
                //printf(" %d ", rand());

                for (int k = 0; k < SIZE; k++)
                {
                    int c = 1;
                    int randnum;

                    while (c != 0)
                    {

                        c = 0;
                        randnum = (rand() % 99) + 1;
                        //printf("%d -- ", randnum);
                        for (int n = 0; n < SIZE; n++)
                        {
                            for (int m = 0; n < SIZE; n++)
                            {
                                if (room->player[i]->board[n][m] == randnum)
                                {
                                    c++;
                                    break;
                                }
                            }
                        }
                    }
                    room->player[i]->board[j][k] = randnum;
                }
            }
        }
    }
}
