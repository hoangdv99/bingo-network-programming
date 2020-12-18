#ifndef __CLIENTHANDLE__
#define __CLIENTHANDLE__
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "protocol.h"

void inputRequest(char *sendbuff);
void createLoginRequest(char *opcode, char *username, char *pass, Request *req);
void createRegisterRequest(char *opcode, char *username, char *pass, char *confirm_pass, Request *req);
void createDetailRequest(char *opcode, Request *req);
void createLogOutRequest(char *opcode, Request *req, char* username);
void createCreateRoomRequest(char *opcode, Request *req);
void createInviteRequest(char *opcode, Request *req, char* username);
void createKickRequest();
int login(int clientfd, char* username, char* password);
int registerAccount(int clientfd, char* username, char* password, char* confirm_pass);
int seeDetail(int clientfd, Request *req, Response *res);
int logOut(int clientfd, char* username);
int createRoom(int clientfd);
int invite(int clientfd, char* username);
int quickJoinClient(int clientfd, Request *req, Response *res);
void test();
#endif