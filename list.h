#ifndef __LIST__
#define __LIST__
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#define PLAYER_MAX 100
#define SIZE 5
#define ROOM_PLAYER_MAX 5

typedef enum USER_STATUS
{
    LOBBY,
    INROOM,
    READY,
    INGAME
} USER_STATUS;

typedef enum ROOM_STATE
{
    NOTSTARTED,
    STARTED
} ROOM_STATE;

typedef struct USER
{
    char username[PLAYER_MAX];
    USER_STATUS status;
    int clientfd;
    int board[SIZE][SIZE];
    struct USER *next;
} USER;

typedef struct ACCOUNT
{
    char username[PLAYER_MAX];
    char password[PLAYER_MAX];
    int status;
    struct ACCOUNT* next;
} ACCOUNT;


typedef struct ROOM
{
    int id;
    USER *host;
    ROOM_STATE status;
    int playerAmount;
    USER *player[ROOM_PLAYER_MAX];
    int picked[SIZE*SIZE];
    int pickedAmount;
    struct ROOM *next;
} ROOM;

// Function -----------------------------------

void readAccountFile();
ACCOUNT* findAccount(char *username);
void printListAccount();
void writeToAccountFile();
void insertAcc(char* username, char* password, int status);

// User init                       
USER *makenode(char *username, int clientfd);   // create new player
void printUser(USER *acc);                       // print player information
USER* insertUser(char *username, int clientfd);   // insert player to user list - return 1 if insert success | 0 if player already in list
void printListUser();                            // print player list
USER *findUserByUsername(char *username);                  // find a player with name - return player node
USER *findUserByClientfd(int clientfd);
int changeUserStatus(int state, char *username); // Change status of a player - return player's state | 0 if that player doesn't exist
USER *deleteUserByUsername(char *username);
USER *deleteUserByClientfd(int clientfd);

// Room init
void insertRoom(int id, USER *host); // create a new room with host name and id - return 1 if success | 0 if fail
ROOM *findRoom(int id);
ROOM *findRoomByClientfd(int clientfd);             
int insertPlayer(int id, USER *player);  // insert a player to a known room - return 1 if success | 0 if room is full
int quickJoin(USER *player);             // auto insert a player to a room - return room id - return that room's id
void printRoomPlayer(int id);        // print all player of a room
void printRoomPlayerBoard(int id, char *name);
int countRoom();
ROOM *deleteRoom(int id);
void printListRoom();
void detelePlayerFromRoom(ROOM *room, USER *user);
int startGame(int id);                  // Start game and gennerrate board for player
int pickNumber(int roomid, int pick);   // Pick number
int checkBingo(int roomid, char *name); // Check Bingo
int BINGOTEST(int roomid, char *name, int choice, int place);  // TEST IF BINGO  --- choice 1 = col; 2 = row; 3 = cross
int playGame(int roomid, fd_set readfds, int master_socket, int max_sd); 
#endif