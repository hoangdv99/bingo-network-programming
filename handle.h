#ifndef __HANDLE__
#define __HANDLE__
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#define MAX_LENGTH 255
#define MAX_CLIENT 20
#define MAX_LENGTH 255
#include "protocol.h"
#include "list.h"
 
void login(USER *userListHead, int clientfd, Request *req, Response *res);
void sendDetail(USER *userListHead, int clientfd, Request *req, Response *res);
void goToLobby(USER *userListHead, int clientfd, Request *req, Response *res);
void backToMenu(USER *userListHead, int clientfd, Request * req, Response *res);
void createRoom(USER *userListHead, ROOM *roomListHead, int clientfd, Request *req, Response *res);
void quickjoin(USER *userListHead, ROOM *roomListHead, int clientfd, Request *req, Response *res);

#endif