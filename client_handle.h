// #ifndef __CLIENTHANDLE__
// #define __CLIENTHANDLE__
// #include <string.h>
// #include <stdlib.h>
// #include <stdio.h>
// #include "protocol.h"

// void inputRequest(char *sendbuff);
// void createLoginRequest(char *opcode, char *username, char *pass, Request *req);
// void createRegisterRequest(char *opcode, char *username, char *pass, char *confirm_pass, Request *req);
// void createDetailRequest(char *opcode, Request *req);
// void createLogOutRequest(char *opcode, Request *req, char* username);
// void createLogOutByXRequest(char *opcode, Request *req, char* username);
// void createCreateRoomRequest(char *opcode, Request *req);
// void createInviteRequest(char *opcode, Request *req, char* username);
// void createQuickJoinClientRequest(char *opcode, Request *req);
// void createJoinClientRequest(char *opcode, Request *req, char* roomID);
// void createAcceptInviteClientRequest(char *opcode, Request *req, char *hostName);
// void createDeclineInviteClientRequest(char *opcode, Request *req, char *hostName);
// void createBackClientRequest(char *opcode, Request *req, char *username);
// void createKickRequest(char *opcode, Request *req, char *username);

// int login(int clientfd, char* username, char* password);
// int registerAccount(int clientfd, char* username, char* password, char* confirm_pass);
// int seeDetail(int clientfd, Request *req, Response *res);
// int logOut(int clientfd, char* username);
// int logOutByX(int clientfd, char* username);
// int createRoom(int clientfd);
// int quickJoinClient(int clientfd);
// int joinClient(int clientfd, char *roomID);
// int inviteClient(int clientfd, char *username);
// int acceptInviteClient(int clientfd, char *hostName);
// int declineInviteClient(int clientfd, char *hostName);
// int backClient(int clientfd, char *username);
// int kickClient(int clientfd, char *username);

// void splitRoomID(char *input, char *username, char *id);
// void splitPlayerAmountUsernameListRoomID(char *input, char *playerAmount, char *roomID, char *usernameList);
// void splitHostName(char *input, char *hostName);
// #endif

//hoang test
#ifndef __CLIENTHANDLE__
#define __CLIENTHANDLE__
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "protocol.h"

void inputRequest(char *sendbuff);
void registerAccount(int clientfd, Request *req, Response *res);
void login(int clientfd ,Request *req, Response *res);
void seeDetail(int clientfd, Request *req, Response *res);
void logOut(int clientfd, Request *req, Response *res);
void createRoom(int clientfd, Request *req, Response *res);
void quickJoinClie(int clientfd, Request *req, Response *res);
void inviteClie(int clientfd, Request *req, Response *res);
void joinClie(int clientfd, Request *req, Response *res);
void outRoomClie(int clientfd, Request *req, Response *res);
void kickClie(int clientfd, Request *req, Response *res);
void acceptInviteClie(int clientfd, Request *req, Response *res);
void closeGameClie(int clientfd, Request *req, Response *res);
void declineInviteClie(int clientfd, Request *req, Response *res);
void playClie(int clientfd, Request *req, Response *res);
void readyClie(int clientfd, Request *req, Response *res);
void unreadyClie(int clientfd, Request *req, Response *res);
void checkReadyClie(int clientfd, Request *req, Response *res);
void bingoClie(int clientfd, Request *req, Response *res);
void pickClie(int clientfd, Request *req, Response *res);
void returnRoomClie(int clientfd, Request *req, Response *res);
void quitGameClie(int clientfd, Request *req, Response *res);
void testClie(int clientfd, Request *req, Response *res);
#endif