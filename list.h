#ifndef __LIST__
#define __LIST__
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include "protocol.h"
#define PLAYER_MAX 10
#define SIZE 5
#define ROOM_PLAYER_MAX 5
#define ROOM_MAX 10

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
    char username[MAX_LENGTH];
    USER_STATUS status;
    int clientfd;
    int board[SIZE][SIZE];
    struct USER *next;
} USER;

typedef struct ACCOUNT
{
    char username[MAX_LENGTH];
    char password[MAX_LENGTH];
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
    int pickedNumbers[SIZE*SIZE];
    int numberPickedAmount;
    struct ROOM *next;
} ROOM;

// Function -----------------------------------

void readAccountFile();
ACCOUNT* findAccount(char *username);
void printListAccount();
void writeToAccountFile();
void insertAcc(char* username, char* password, int status);

// User init                       // create new player
void printUser(USER *acc);                       // print player information
USER* insertUser(char *username, int clientfd);   // insert player to user list - return 1 if insert success | 0 if player already in list
void printListUser();                            // print player list
USER *findUserByUsername(char *username);                  // find a player with name - return player node
USER *findUserByClientfd(int clientfd);
int changeUserStatus(int state, char *username); // Change status of a player - return player's state | 0 if that player doesn't exist
USER *deleteUserByUsername(char *username);
USER *deleteUserByClientfd(int clientfd);
int countUser();

// Room init
int insertRoom(ROOM *room); // create a new room with host name and id - return 1 if success | 0 if fail
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
#endif