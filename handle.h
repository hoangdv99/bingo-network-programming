#ifndef __HANDLE__
#define __HANDLE__
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#define MAX_LENGTH 1024
#define MAX_CLIENT 20
#include "protocol.h"
#include "list.h"
#include "helper.h"
#include "gameplay.h"

typedef struct THREAD_DATA{
    int clientfd;
    fd_set readfds;
    int master_socket;
    int max_sd;
} THREAD_DATA;

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
void acceptInvite(int clientfd, Request *req, Response *res);
void declineInvite(int clientfd, Request *req, Response *res);
void ready(int clientfd, Request *req, Response *res);
void unready(int clientfd, Request *req, Response *res);
int checkReady(int clientfd);
void startGame(int sockfd, fd_set readfds, int max_fd ,int clientfd, Request *req, Response *res);
void play(THREAD_DATA threadData, int clientfd, Request *req, Response *res);
#endif