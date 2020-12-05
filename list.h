#ifndef __LIST__
#define __LIST__
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#define MAX 100
#define SIZE 5
#define ROOM_MAX 5

typedef enum USER_STATUS
{
    MENU,
    LOBBY,
    INROOM,
    READY
} USER_STATUS;

typedef enum ROOM_STATE
{
    NOTSTARTED,
    STARTED
} ROOM_STATE;

typedef struct USER
{
    char username[MAX];
    USER_STATUS status;
    int clientfd;
    int board[SIZE][SIZE];
    struct USER *next;
} USER;
USER *userListHead = NULL;

typedef struct ROOM
{
    int id;
    USER *host;
    ROOM_STATE status;
    int playerAmount;
    USER *player[ROOM_MAX];
    struct ROOM *next;
} ROOM;
ROOM *roomListHead = NULL;

// Function -----------------------------------
// User init                       // create new player
void printUser(USER *acc);                       // print player information
USER* insertUser(char *username, int clientfd);   // insert player to user list - return 1 if insert success | 0 if player already in list
void printListUser();                            // print player list
USER *findUser(char *username);                  // find a player with name - return player node
int changeUserStatus(int state, char *username); // Change status of a player - return player's state | 0 if that player doesn't exist
USER *deleteUser(char *username);
// Room init
void insertRoom(int id, USER *host); // create a new room with host name and id - return 1 if success | 0 if fail
ROOM *findRoom(int id);              // find a room with it's id - return room node
int insertPlayer(int id, USER *player);  // insert a player to a known room - return 1 if success | 0 if room is full
int quickJoin(USER *player);             // auto insert a player to a room - return room id - return that room's id
void printRoomPlayer(int id);        // print all player of a room
void printRoomPlayerBoard(int id, char *name);

#endif