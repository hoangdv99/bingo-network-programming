#include "handle.h"
#define FILE_SIZE 1024

void login(USER *userListHead, int clientfd ,Request *req, Response *res) //neu username da ton tai thi bao loi, username chua ton tai, cung clienfd thi thay doi username=>xoa username cu
{
    char username[MAX_LENGTH];
    USER *user;

    strcpy(username, req->message);
    if ((findUserByUsername(userListHead, username)) != NULL)
    {
        res->code = USERNAME_EXISTED;
        strcpy(res->data, username);
        setMessageResponse(res);
    }else
    {
        user = findUserByClientfd(userListHead, clientfd);
        if(user != NULL){
            deleteUser(userListHead, user->username);
        }
        insertUser(userListHead, username, clientfd);
        res->code = LOGIN_SUCCESS;
        strcpy(res->data, username);
        setMessageResponse(res); 
    }
    //printListUser(userListHead);
    sendRes(clientfd, res, sizeof(Response), 0);
}

void sendDetail(USER *userListHead, int clientfd, Request *req, Response *res){
    res->code = RES_DETAIL;
    setMessageResponse(res);
    sendRes(clientfd, res, sizeof(Response), 0);
}

void goToLobby(USER *userListHead, int clienfd, Request *req, Response *res){
    USER *currUser = findUserByClientfd(userListHead, clienfd);
    if(currUser == NULL){
        res->code = UNAUTH;
        setMessageResponse(res);
        sendRes(clienfd, res, sizeof(Response), 0);
    }else{
        currUser->status = LOBBY;
        res->code = RES_PLAY;
        setMessageResponse(res);
        sendRes(clienfd, res, sizeof(Response), 0);
    }
}

void backToMenu(USER *userListHead, int clientfd, Request *req, Response *res){
    USER *currUser = findUserByClientfd(userListHead, clientfd);
    currUser->status = MENU;
    res->code = RES_BACK_TO_MENU;
    setMessageResponse(res);
    sendRes(clientfd, res, sizeof(Response), 0);
}

void createRoom(USER *userListHead, ROOM *roomListHead, int clientfd, Request *req, Response *res){
    ROOM *newRoom = (ROOM*)malloc(sizeof(ROOM));
    USER *host = findUserByClientfd(userListHead, clientfd);
    int roomID = countRoom(roomListHead) + 1;
    insertRoom(roomListHead, roomID, host);
    insertPlayer(roomListHead, roomID, host);
    res->code = RES_CREATE_ROOM;
    setMessageResponse(res);
    sendRes(clientfd, res, sizeof(Response), 0);
}

void quickjoin(USER *userListHead, ROOM *roomListHead, int clientfd, Request *req, Response *res){
    USER *user = findUserByClientfd(userListHead, clientfd);
    char dataString[MAX_LENGTH];
    if(quickJoin(roomListHead, user) == -1){
        res->code = QUICKJOIN_FAIL;
        setMessageResponse(res);
        sendRes(clientfd, res, sizeof(Response), 0);
    }else{
        res->code = QUICKJOIN_SUCCESS;
        sprintf(dataString, "%d", quickJoin(roomListHead, user)); //room ID -> string
        strcpy(res->data, dataString);
        setMessageResponse(res);
        sendRes(clientfd, res, sizeof(Response), 0);
    }
}