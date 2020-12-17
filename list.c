#include "list.h"
#include "helper.h"
#define ROOM_MAX 100
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

USER *makenode(char *username, int clientfd) {
    USER *acc = (USER *)malloc(sizeof(USER));
    strcpy(acc->username, username);
    acc->clientfd = clientfd;
    acc->status = LOBBY;
    for(int i = 0; i < SIZE; i++) {
        for(int j = 0; j < SIZE; j++) {
            acc->board[i][j] = 0;
        }
    }
    return acc;
}

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
        printf("%s %d\n", ptr->username, ptr->clientfd);
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
    newRoom->player[0] = host;
    newRoom->playerAmount = 1;
    newRoom->pickedAmount = 0;
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

ROOM *findRoomByClientfd(int clientfd){
    ROOM *curr = roomListHead;
    if(curr == NULL){
        return NULL;
    }
    while((findInArray(clientfd, curr->player, curr->playerAmount)) == 0){
        if(curr->next == NULL)  return NULL;
        else    curr = curr->next;
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

void detelePlayerFromRoom(ROOM *room, USER* user){
    int i;

    for(i = 0; i < room->playerAmount; i++){
        if(room->player[i] == user) break;
    }
    if(i < room->playerAmount){
        room->playerAmount = room->playerAmount - 1;
        for (int j = i; j < room->playerAmount; j++)
        {
            room->player[j] = room->player[j+1];
        }
    }
    user->status = LOBBY;
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

void printRoomPlayer(int id) {
    ROOM *room = findRoom(id);
    if(room != NULL) {
        printf("Room[%d] player\n----\n", id);
        for(int i = 0; i < room->playerAmount; i++) {
            printf("Player[%d]: %s\n", i+1, room->player[i]->username);
        }
    }   
    else {
        printf("Room not exist!\n");
    }
}

void printRoomPlayerBoard(int id, char *name)
{
    ROOM *room = findRoom(id);
    if(room != NULL) {
    //printf(" %d Player\n", room->playerAmount);
        for(int i = 0; i < room->playerAmount; i++) {           
            if(strcmp(name, room->player[i]->username) == 0) {
                printf("---------\nPlayer %s's board\n", room->player[i]->username);
                int j,k;
                for(j = 0; j < SIZE; j++) {
                    for(k = 0; k < SIZE; k++) {
                        printf("%d\t", room->player[i]->board[j][k]);
                    }
                    printf("\n");
                }
            }
        }
    }
    else {
        printf("Room not exist!\n");
    }
    
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
    return 0;
}

ROOM *deleteRoom(int id){
    ROOM *cur = roomListHead;
    ROOM *prev = NULL;
    if (roomListHead == NULL)
        return NULL;
    while (cur->id != id)
    {
        if (cur->next == NULL)
            return NULL;
        else
        {
            prev = cur;
            cur = cur->next;
        }
    }
    if (cur == roomListHead)
    {
        roomListHead = roomListHead->next;
    }
    else
    {
        prev->next = cur->next;
    }
    return cur;
}

void printListRoom(){
    ROOM *ptr = roomListHead;
    while (ptr != NULL)
    {
        printf("%d-%s-%d\n", ptr->id, ptr->host->username, ptr->playerAmount);
        printRoomPlayer(ptr->id);
        ptr = ptr->next;
    }
}

// PLAY FUNCTION

int startGame(int id)
{
    ROOM *room = findRoom(id);
    if (room == NULL)
    {
        printf("Room not exit");
        return 0;
    }
    else
    {
        printf("Board generating!\n");
        int n = 0;
        srand(time(NULL));
        for (n = 0; n < room->playerAmount; n++)
        {
            int a[SIZE*SIZE],i,j,x,y;
            x = 0;
            y = 0;
            int temp;

            
            for(i = 0; i < SIZE*SIZE; i++) {
                temp = rand()%(SIZE*SIZE)+1;
                for(j = 0; j < i; j++) {
                    if(temp == a[j]) {
                        break;
                    }
                }
                if(i == j) {
                    a[i] = temp;
                }
                else {
                    i--;
                }
            }
            //printf("\n  %s  \n", room->player[n]->username);
            for(i=0;i<SIZE*SIZE;i++) {
                room->player[n]->board[x][y] = a[i]; 
                if((i+1) % 5 == 0) {
                    x++;
                    y = 0;
                    //printf("%d", x);
                }
                else {
                    y++;
                }
            }

        }
        printf("Board generated!\n");
        return 1;
    }
}

int pickNumber(int roomid, int pick) {
    ROOM *room = findRoom(roomid);
    if (room == NULL)
    {
        printf("Room not exit");
        return 0;
    }
    else {
        int picked = 0;
        if(room->pickedAmount == 25) {
            printf("Out of choice!\n");
            return 0;
        } 
        else {
            int i;
            for(i = 0; i < room->pickedAmount; i++) {
                if (pick == room->picked[i]) {
                    return 0;
                }
            }
            room->picked[room->pickedAmount] = pick;
            room->pickedAmount++;
            
            for(int i = 0; i < room->playerAmount; i++) {          
                int j,k;
                for(j = 0; j < SIZE; j++) {
                    for(k = 0; k < SIZE; k++) {
                        if (room->player[i]->board[j][k] == pick) {
                            room->player[i]->board[j][k] = 0;
                            picked++;
                        }
                    }
                    
                }
                
            }
        }
        return picked;
    }    
}

int checkBingo(int roomid, char *name) {
    ROOM *room = findRoom(roomid);
    if(room != NULL) {
    
        for(int i = 0; i < room->playerAmount; i++) {           
            if(strcmp(name, room->player[i]->username) == 0) {
                int j,k;
                int sum_row, sum_col;
                int cross_1 = 0; 
                int cross_2 = 0;
                
                for(j = 0; j < SIZE; j++) {
                    sum_row = 0;
                    sum_col = 0;
                    for(k = 0; k < SIZE; k++) {
                        //printf("%d\t", room->player[i]->board[j][k]);
                        sum_row += room->player[i]->board[j][k];
                        sum_col += room->player[i]->board[k][j];
                        if(j == k) {
                            cross_1 += room->player[i]->board[j][k];
                        }
                        if((j+k) == 4) {
                            cross_2 += room->player[i]->board[j][k];
                        }
                    }
                    if((sum_col == 0) || (sum_row == 0) || (cross_1 == 0) || (cross_2 == 0)) {
                        printf("Player %s BINGO\n", name);
                        return 1;
                    }
                    //printf("\n");
                }
            }
        }
        return 0;
    }
    else {
        printf("Room not exist!\n");
        return 0;
    }
}

//TEST 

int BINGOTEST(int roomid, char *name, int choice, int place) {
    ROOM *room = findRoom(roomid);
    if (room == NULL)
    {
        printf("Room not exit");
        return 0;
    }
    else {
        for(int n = 0; n < room->playerAmount; n++) {
            if(strcmp(name, room->player[n]->username) == 0) {
                int i,j;
                switch (choice)
                {
                case 1: {
                    for(i = 0; i < SIZE; i++) {
                        room->player[n]->board[i][place] = 0;
                    }
                } break;
                case 2: {
                    for(i = 0; i < SIZE; i++) {
                        room->player[n]->board[place][i] = 0;
                    }
                } break;
                case 3: {
                    for(i = 0; i < SIZE; i++) {
                        for(j = 0; j < SIZE; j++) {
                            if((i+j) == (SIZE-1) && place == 2) {
                                room->player[n]->board[i][j] = 0;
                            }
                            if((i == j) && place == 1) {
                                room->player[n]->board[i][j] = 0;
                            }
                        }
                    }
                } break;
                
                default: {
                    return 0;
                } break;
                }
            }
        }
    }
    return 1;
}

int playGame(int roomid, fd_set readfds, int master_socket, int max_sd) {
    return 1;
}