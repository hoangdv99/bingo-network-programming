#ifndef __CLIENTHANDLE__
#define __CLIENTHANDLE__
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#define MAX_LENGTH 255
#include "protocol.h"

void inputRequest(char *sendbuff);
void registerAccount(int clientfd, Request *req, Response *res);
void login(int clientfd ,Request *req, Response *res);
void seeDetail(int clientfd, Request *req, Response *res);
void logOut(int clientfd, Request *req, Response *res);
void createRoom(int clientfd, Request *req, Response *res);
void quickJoin(int clientfd, Request *req, Response *res);
#endif