#ifndef __PROTOCOL__
#define __PROTOCOL__
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#define MAX_LENGTH 255

typedef enum REQ_OPCODE{
    REGISTER,
    LOGIN,
    DETAIL,
    LOGOUT,
    CREATE_ROOM,
    INVITE,
    ACCEPT_INVITE_REQUEST,
    DECLINE_INVITE_REQUEST,
    KICK,
    QUICKJOIN,
    JOIN,
    OUT_ROOM,
    CLOSE,
    PLAY,
    BINGO,
    PICK,
    READY1,
    UNREADY,
    CHECK_READY,
    RETURN_ROOM,
    QUIT,
    TEST
}REQ_OPCODE; 

typedef enum RES_OPCODE{
    ROOM_CHANGED,
    SYNTAX_ERROR,
    REGISTER_INPUT_WRONG,
    USERNAME_EXISTED,
    REGISTER_SUCCESS,
    USERNAME_NOT_EXISTED,
    LOGIN_SUCCESS,
    LOGOUT_SUCCESS,
    WRONG_PASSWORD,
    ACCOUNT_BUSY,
    RES_DETAIL,
    CREATE_ROOM_SUCCESS,
    INVITATION,
    INVITE_SUCCESS,
    INVITE_FAIL,
    QUICKJOIN_FAIL,
    QUICKJOIN_SUCCESS,
    JOIN_SUCCESS,
    JOIN_FAIL,
    ROOM_FULL,
    NEW_HOST,
    OUT_ROOM_SUCCESS,
    KICK_SUCCESS,
    KICK_FAIL,
    BE_KICKED,
    ACCEPTED,
    DECLINED,
    GAME_START,
    YOUR_TURN,
    OTHER_PLAYER_TURN,
    BINGO_REAL,
    BINGO_FAKE,
    PICK_FAIL,
    PICK_SUCCESS,
    YOU_WIN,
    OTHER_PLAYER_WIN,
    NO_ROOM,
    READY_SUCCESS,
    UNREADY_SUCCESS,
    ALL_PLAYERS_READY,
    SOMEONE_UNREADY,
    PLAYER_NOT_ENOUGH,
    NEW_PLAYER_JOINED,
    SOMEONE_LEFT_GAME,
    ALL_PLAYERS_LEFT_GAME,
    RETURN_ROOM_SUCCESS,
    DISCONNECTED,
    ACCEPT_INVITE,
    DECLINE_INVITE,
    BOARD_DATA_GENERATED
}RES_OPCODE;

typedef struct Request
{
    REQ_OPCODE code;
    char message[MAX_LENGTH];
} Request;

typedef struct Response
{   
    RES_OPCODE code;
    char message[MAX_LENGTH];
    char data[MAX_LENGTH];
} Response;

int recvReq(int socket, Request *buff, int size, int flags); 
int sendReq(int socket, Request *buff, int size, int flags);

int sendRes(int socket, Response *msg, int size, int flags);
int recvRes(int socket, Response *msg, int size, int flags);

void setMessageResponse(Response *msg);
void readMessageResponse(Response *msg);

void setOpcodeRequest(Request *request, char *input);

int sendNum(int socket, int num, int size, int flags);
int recvNum(int socket, int num, int size, int flags);

void logout(int socket, char *username);

#endif