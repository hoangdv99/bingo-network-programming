
#include <stdio.h> 
#include <string.h> 
#include <stdlib.h> 
#include <stdbool.h>
#include<time.h> 

#define MAX 100
#define SIZE 5
#define ROOM_MAX 5

typedef enum {
    MENU, 
    LOBBY, 
    ROOM,
    READY
} userstatus;

typedef enum {
    NOTSTART,
    START
} roomstate;

typedef struct user {
    char username[MAX];
    userstatus status;
    int clientfd;
    int board[SIZE][SIZE];
} user;

typedef struct user_list {
    user user;
    struct user_list *next;
} user_list;

typedef user_list *listnodeptr;
listnodeptr accounts = NULL;

typedef struct room_list {
    int id;
    char hostname[MAX];
    roomstate status;
    int playernumber;
    user player[ROOM_MAX];
    struct room_list *next;
} room_list;

typedef room_list *listroomptr;
listroomptr rooms = NULL;

// Function -----------------------------------
// User init
user makenode(char *username, int clientfd);                        // create new player
void printnode(user acc);                                           // print player information
int insertlast(listnodeptr *sPtr, user acc);                        // insert player to user list - return 1 if insert success | 0 if player already in list
void printlistuser(listnodeptr currentPtr);                         // print player list
listnodeptr findnode(listnodeptr sPtr, char *username);             // find a player with name - return player node
int changestate(listnodeptr sPtr, int state, char *username);       // Change status of a player - return player's state | 0 if that player doesn't exist

// Room init
int insertroom(listroomptr *sPtr, int id, char *hostname, user acc);// create a new room with host name and id - return 1 if success | 0 if fail
listroomptr findroom(listroomptr rPtr, int id);                     // find a room with it's id - return room node
int insertplayer(listroomptr *sPtr, int id, user acc);              // insert a player to a known room - return 1 if success | 0 if room is full
int quickjoin(listroomptr *sPtr, user acc);                         // auto insert a player to a room - return room id - return that room's id
void printroomplayer (listroomptr roomPtr);                         // print all player of a room
void printroomplayerboard (listroomptr roomPtr, char *name);

// Play
int playgame(int id);                // bingo board generator


// Function --------------------------------------=======-----------------------------------
// USER FUNCTION
user makenode(char *username, int clientfd) {
    user acc;
    strcpy(acc.username, username);
    acc.clientfd = clientfd;
    acc.status = LOBBY;
    for(int i = 0; i < SIZE; i++) {
        for(int j = 0; j < SIZE; j++) {
            acc.board[i][j] = 0;
        }
    }
    return acc;
}
void printnode(user acc) {
    printf("%s-%d || %d\n------------------------------------\n",
    acc.username,acc.clientfd,acc.status);
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

int insertlast(listnodeptr *sPtr, user acc) {
    if(findnode(*sPtr, acc.username) == NULL) {
        listnodeptr newPtr, currentPtr, prevPtr;

        newPtr = malloc(sizeof(user_list));
        newPtr->user = acc;
        newPtr->next = NULL;

        currentPtr = *sPtr;
        prevPtr = NULL;

        while (currentPtr != NULL) {
            prevPtr = currentPtr;
            currentPtr = currentPtr->next;
        }

        if (prevPtr == NULL) {
            newPtr->next = *sPtr;
            *sPtr = newPtr;
        } else {
            prevPtr->next = newPtr;
            newPtr->next = currentPtr;
        }
        return 1;
    }
    return 0;
}

void printlistuser(listnodeptr currentPtr) {
  while (currentPtr != NULL) {
    printnode(currentPtr->user);
    currentPtr = currentPtr->next;
  }
}

listnodeptr findnode(listnodeptr sPtr, char *username) {
  if (sPtr == NULL) {
    return NULL;
  }
  if (strcmp(sPtr->user.username, username) == 0) {
    return sPtr;
  } else {
    findnode(sPtr->next, username);
  }
}

// ROOM FUNCTION

int changestate(listnodeptr sPtr, int state, char *username) {
    while (sPtr != NULL) {
        if(strcmp(sPtr->user.username, username) == 0 ) {
        sPtr->user.status = state;
        //printf("%d - \n",sPtr->user.status);
        return state;
        }
    sPtr = sPtr->next;
  }
  return -1;
}

int insertroom(listroomptr *rPtr, int id, char *hostname, user acc) {
    if(findroom(*rPtr, id) == NULL) {
        listroomptr newPtr, currentPtr, prevPtr;

        newPtr = malloc(sizeof(room_list));
        newPtr->id = id;
        strcpy(newPtr->hostname, hostname);
        newPtr->playernumber = 1;
        newPtr->status = NOTSTART;
        newPtr->next = NULL;
        acc.status = ROOM;
        int i = changestate(accounts, ROOM, hostname);
        if (i == -1) 
            printf("fails\n");

        newPtr->player[0] = acc;
        

        currentPtr = *rPtr;
        prevPtr = NULL;

        while (currentPtr != NULL) {
            prevPtr = currentPtr;
            currentPtr = currentPtr->next;
        }

        if (prevPtr == NULL) {
            newPtr->next = *rPtr;
            *rPtr = newPtr;
        } else {
            prevPtr->next = newPtr;
            newPtr->next = currentPtr;
        }
        return 1;
    }
    return 0;
}

listroomptr findroom(listroomptr rPtr, int id) {
    if (rPtr == NULL) {
        //printf("Room has not created!\n");
        return NULL;
    }
    if (rPtr->id == id) {
        return rPtr;
    } else {
        findroom(rPtr->next, id);
    }
}

int insertplayer(listroomptr *sPtr, int id, user acc) {
    listroomptr foundPtr = findroom(rooms, id);

    if (foundPtr != NULL && foundPtr->playernumber < ROOM_MAX) {
        acc.status = ROOM;
        foundPtr->player[foundPtr->playernumber] = acc;
        foundPtr->playernumber++;
        
        int i = changestate(accounts, ROOM, acc.username);
        if (i == -1) {
            //printf("%d - %s- fails\n",i, acc.username);
        }
        else {
            //printf("%s- done\n", acc.username);
        }

        listnodeptr Ptr = findnode(accounts, acc.username);
        if (Ptr == NULL) {
            printf("%s- fails\n", acc.username);
        }
        return 1;
    }
    else {
        printf("Room is full!\n");
        return 0;
    }
}

int quickjoin(listroomptr *rPtr, user acc) {
    listroomptr currentPtr = *rPtr;
    int id = 1;
    while (currentPtr != NULL) {
        if(currentPtr->playernumber < ROOM_MAX) {
            insertplayer(&currentPtr, currentPtr->id, acc);
            return currentPtr->id;
        }
        id = currentPtr->id;
        currentPtr = currentPtr->next;     
    }
    id++;
    printf("All room are full!\nCreating new room %d\n", id);
    int i = insertroom(rPtr, id, acc.username, acc);
    if(i==1) {
        printf("New room create\n");
    }
    //insertplayer(rPtr, id, acc);
    return id;
}

void printroomplayer (listroomptr roomPtr) {
    if (roomPtr == NULL) {
        printf("Room has not created!\n");
        return;
    }
    printf("ROOM %d --\n-------\n", roomPtr->id);
    for(int i = 0; i < roomPtr->playernumber; i++) {
        printnode(roomPtr->player[i]);
    }
    printf("\n");
}

void printroomplayerboard (listroomptr roomPtr, char *name) {
    if (roomPtr == NULL) {
        printf("Room has not created!\n");
        return;
    }
    printf("Player %s --\n-------\n", name);
    for(int i = 0; i < roomPtr->playernumber; i++) {
        if (strcmp(roomPtr->player[i].username, name) == 0 ) {
            for(int k = 0; k < SIZE; k++) {
                for(int j = 0; j < SIZE; j++) {
                    printf("%3d\t", roomPtr->player[i].board[k][j] );
                }
                printf("\n");
            }
        }
    }
    printf("\n");
}

// PLAY FUNCTION 

int playgame(int id) {
    listroomptr foundPtr = findroom(rooms, 1);
    if (foundPtr == NULL) {
        printf ("Room not exit");
    }
    else {
        int i = 0;
        srand(time(0));  
        for(i = 0; i < foundPtr->playernumber; i++) {
             
            for(int j = 0; j < SIZE; j++) {
                //printf(" %d ", rand());
                
                for(int k = 0; k < SIZE; k++) {
                    int c = 1;
                    int randnum;
                    
                    while (c != 0)
                    {             
                        
                        c = 0;      
                        randnum = (rand() % 99) + 1;
                        //printf("%d -- ", randnum);
                        for(int n = 0; n < SIZE; n++) {
                           for(int m = 0; n < SIZE; n++) {
                               if(foundPtr->player[i].board[n][m] == randnum) {
                                   c++;
                                   break;
                               }
                           }
                        }
                    }
                    foundPtr->player[i].board[j][k] = randnum;                  
                }
            }
        }
    }
    
}
