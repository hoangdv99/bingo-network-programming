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
    QUICKJOIN
}REQ_OPCODE; 

typedef enum RES_OPCODE{
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
    QUICKJOIN_FAIL,
    QUICKJOIN_SUCCESS
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