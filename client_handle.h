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
void createLogOutByXRequest(char *opcode, Request *req, char* username);
void createCreateRoomRequest(char *opcode, Request *req);
void createInviteRequest(char *opcode, Request *req, char* username);
void createQuickJoinClientRequest(char *opcode, Request *req);
void createJoinClientRequest(char *opcode, Request *req, char* roomID);
void createAcceptInviteClientRequest(char *opcode, Request *req, char *hostName);
void createDeclineInviteClientRequest(char *opcode, Request *req, char *hostName);
void createBackClientRequest(char *opcode, Request *req, char *username);
void createKickRequest(char *opcode, Request *req, char *username);
void createReadyClientRequest(char *opcode, Request *req, char *username);
void createUnReadyClientRequest(char *opcode, Request *req, char *username);
void createStartGameClientRequest(char *opcode, Request *req, char *username);
void createPickClientRequest(char *opcode, Request *req, char *pickedNumber);
void createBingoClientRequest(char *opcode, Request *req, char *boardString);
void createPlayingBackClientRequest(char *opcode, Request *req, char *username);
void createPlayingQuitClientRequest(char *opcode, Request *req, char *username);

int login(int clientfd, char* username, char* password);
int registerAccount(int clientfd, char* username, char* password, char* confirm_pass);
int seeDetail(int clientfd, Request *req, Response *res);
int logOut(int clientfd, char* username);
int logOutByX(int clientfd, char* username);
int createRoom(int clientfd);
int quickJoinClient(int clientfd);
int joinClient(int clientfd, char *roomID);
int inviteClient(int clientfd, char *username);
int acceptInviteClient(int clientfd, char *hostName);
int declineInviteClient(int clientfd, char *hostName);
int backClient(int clientfd, char *username);
int kickClient(int clientfd, char *username);
int readyClient(int clientfd, char *username);
int unReadyClient(int clientfd, char *username);
int startGameClient(int clientfd, char *username);
int pickClient(int clientfd, char *pickedNumber);
int bingoClient(int clientfd, char *boardString);
int playingBackClient(int clientfd, char *username);
int playingQuitClient(int clientfd, char *username);

void splitRoomID(char *input, char *username, char *id);
void splitPlayerAmountUsernameListRoomID(char *input, char *playerAmount, char *roomID, char *usernameList);
void splitHostName(char *input, char *hostName);
void split2Board(char *input, char *normalBoard, char *wonBoard);
#endif

