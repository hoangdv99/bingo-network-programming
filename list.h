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

typedef struct ROOM
{
    int id;
    USER *host;
    ROOM_STATE status;
    int playerAmount;
    USER *player[ROOM_MAX];
    struct ROOM *next;
} ROOM;

// Function -----------------------------------
// User init                       // create new player
void printUser(USER *userListHead,USER *acc);                       // print player information
USER* insertUser(USER *userListHead, char *username, int clientfd);   // insert player to user list - return 1 if insert success | 0 if player already in list
void printListUser(USER *userListHead);                            // print player list
USER *findUserByUsername(USER *userListHead, char *username);                  // find a player with name - return player node
USER *findUserByClientfd(USER *userListHead, int clientfd);
int changeUserStatus(USER *userListHead, int state, char *username); // Change status of a player - return player's state | 0 if that player doesn't exist
USER *deleteUser(USER *userListHead, char *username);
// Room init
void insertRoom(ROOM *roomListHead, int id, USER *host); // create a new room with host name and id - return 1 if success | 0 if fail
ROOM *findRoom(ROOM *roomListHead, int id);              // find a room with it's id - return room node
int insertPlayer(ROOM *roomListHead, int id, USER *player);  // insert a player to a known room - return 1 if success | 0 if room is full
int quickJoin(ROOM *roomListHead, USER *player);             // auto insert a player to a room - return room id - return that room's id
void printRoomPlayer(ROOM *roomListHead, int id);        // print all player of a room
void printRoomPlayerBoard(ROOM *roomListHead, int id, char *name);
int countRoom(ROOM *roomListHead);
#endif