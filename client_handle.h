#ifndef __CLIENTHANDLE__
#define __CLIENTHANDLE__
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#define MAX_LENGTH 255
#include "protocol.h"

void inputRequest(char *sendbuff);
void createLoginRequest(char *opcode, char *username, char *pass, Request *req);
void createRegisterRequest(char *opcode, char *username, char *pass, char *confirm_pass, Request *req);
void createDetailRequest(char *opcode, Request *req);
void createLogOutRequest(char *opcode, Request *req);
int registerAccount(int clientfd, Request *req, Response *res);
int login(int clientfd, Request *req, Response *res);
int seeDetail(int clientfd, Request *req, Response *res);
int logOut(int clientfd, Request *req, Response *res);
int createRoom(int clientfd, Request *req, Response *res);
int quickJoin(int clientfd, Request *req, Response *res);
#endif