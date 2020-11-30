#ifndef __PROTOCOL__
#define __PROTOCOL__
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "gameplay.h"
#include "helper.h"
#define MAX_LENGTH 255
typedef enum{
    LOGIN,
    BACKTOMENU,
    MAKEROOM,
    JOIN,
    QUICKJOIN,
    ACCEPT,
    REFUSE,
    BACKTOLOBBY,
    START,
    INVITE,
    KICK,
    SELECT,
    BINGO,
    QUITGAME,
    CLOSE
}OPCODE;

typedef enum{
    SYNTAX_ERROR,
    USERNAME_EXISTED,
    LOGIN_SUCCESS 
}MESSAGE_STATUS;

typedef struct Request
{
    OPCODE code;
    char message[MAX_LENGTH];
}Request;

typedef struct Response
{   
    MESSAGE_STATUS code;
    GAMEPLAY_STATUS status;
    char message[50];
    char data[50];
}Response;

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