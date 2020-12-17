#ifndef __HANDLE__
#define __HANDLE__
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#define MAX_LENGTH 1024
#define MAX_CLIENT 20
#include "protocol.h"
#include "list.h"
#include "helper.h"

void signUp(int clientfd, Request *req, Response* res);
void logIn(int clientfd, Request *req, Response *res);
void logOut(int clientfd, Request *req, Response *res);
void sendDetail(int clientfd, Request *req, Response *res);
void createRoom(int clientfd, Request *req, Response *res);
void quickjoin(int clientfd, Request *req, Response *res);
void join(int clientfd, Request *req, Response *res);
void sendInvite(int clientfd, Request *req, Response *res);
void kick(int clientfd, Request *req, Response *res);
void outRoom(int clientfd, Request *req, Response *res);
void exitGame(int clientfd, Request *req, Response *res);
#endif