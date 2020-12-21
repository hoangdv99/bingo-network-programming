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
void exitGameCLie(int clientfd, Request *req, Response *res);
void declineInviteClie(int clientfd, Request *req, Response *res);
void playClie(int clientfd, Request *req, Response *res);

#endif