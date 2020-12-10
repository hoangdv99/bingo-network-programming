#include "list.h"
extern ACCOUNT *accountListHead;
extern USER *userListHead;
extern ROOM *roomListHead;

void printListAccount()
{
    ACCOUNT *ptr = accountListHead;
    while (ptr != NULL)
    {
        printf("%s-%s\n", ptr->username, ptr->password);
        ptr = ptr->next;
    }
}


void insertAcc(char* username, char *password, int status){
    ACCOUNT *acc = (ACCOUNT*)malloc(sizeof(ACCOUNT));
    strcpy(acc->username, username);
    strcpy(acc->password, password);
    acc->status = status;
    if(accountListHead == NULL){
        accountListHead = acc;
    }else{
        acc->next = accountListHead;
        accountListHead = acc;
    }
}

ACCOUNT *findAccount(char *username)
{
    ACCOUNT *curr = accountListHead;
    if (accountListHead == NULL)
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

void readAccountFile()
{
    FILE *f;
    char username[255], password[255];
    int status;

    if ((f = fopen("accounts.txt", "r")) == NULL)
    {
        printf("Cannot open file\n");
        return;
    }
    while (fscanf(f, "%s\t%s\t%d\n", username, password, &status) != EOF)
    {
        insertAcc(username, password, status);
    }
    fclose(f);
}


void writeToAccountFile()
{
    // Open account file for reading
    FILE *file;
    if ((file = fopen("accounts.txt", "w")) == NULL)
    {
        printf("File not found!\n");
        return;
    }

    // Loop for list to write node to file
    ACCOUNT *tmp = accountListHead;
    while (tmp != NULL)
    {
        fprintf(file, "%s  %s  %d\n", tmp->username, tmp->password, tmp->status);
        tmp = tmp->next;
    }

    // Close file
    fclose(file);
}

//---------------------------------------------------

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


USER *insertUser(char *username, int clientfd)
{
    if (findUserByUsername(username) != NULL)
    {
        return NULL;
    }
    else
    {
        USER *user = (USER *)malloc(sizeof(USER));
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

USER *findUserByUsername(char *username)
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

USER *findUserByClientfd(int clientfd)
{
    USER *curr = userListHead;
    if (userListHead == NULL)
        return NULL;
    while (curr->clientfd != clientfd)
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

USER *deleteUserByUsername(char *username)
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

USER *deleteUserByClientfd(int clientfd)
{
    USER *cur = userListHead;
    USER *prev = NULL;
    if (userListHead == NULL)
        return NULL;
    while (cur->clientfd != clientfd)
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
    return 0;
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

int countRoom()
{
    int count = 0;
    ROOM *curr = roomListHead;
    if (curr == NULL)
        return 0;
    while (curr != NULL)
    {
        if (curr->next == NULL)
            return count;
        else
        {
            curr = curr->next;
            count++;
            return count;
        }
    }
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
