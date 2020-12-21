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
void createQuickJoinClientRequest(char *opcode, Request *req);
void createJoinClientRequest(char *opcode, Request *req, char* roomID);
void createKickRequest();

int login(int clientfd, char* username, char* password);
int registerAccount(int clientfd, char* username, char* password, char* confirm_pass);
int seeDetail(int clientfd, Request *req, Response *res);
int logOut(int clientfd, char* username);
int createRoom(int clientfd);
int invite(int clientfd, char* username);
int quickJoinClient(int clientfd);
int joinClient(int clientdfd, char *roomID);
void splitRoomID(char *input, char *username, char *id);
#endif