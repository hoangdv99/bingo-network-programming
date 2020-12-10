#include "handle.h"
#define FILE_SIZE 1024
extern ACCOUNT *accountListHead;
extern USER *userListHead;
extern ROOM *roomListHead;

void signUp(int clientfd, Request *req, Response *res){
    char *username, *password, *confirmPassword;
    
    username = strtok(req->message, "@");
    password = strtok(NULL, "@");
    confirmPassword = strtok(NULL, "\0");

    if(strcmp(username, "") == 0 || strcmp(password, "") ==0 
    || strcmp(confirmPassword, "") == 0 || strcmp(password, confirmPassword) != 0){
        res->code = REGISTER_INPUT_WRONG;
        setMessageResponse(res);
        sendRes(clientfd, res, sizeof(Response), 0);
        return;
    }
    if((findAccount(username)) != NULL){
        res->code = USERNAME_EXISTED;
        setMessageResponse(res);
        sendRes(clientfd, res, sizeof(Response), 0);
        return;
    }
    insertAcc(username, password, 1);
    writeToAccountFile();
    res->code = REGISTER_SUCCESS;
    setMessageResponse(res);
    sendRes(clientfd, res, sizeof(Response), 0);
}

void logIn(int clientfd ,Request *req, Response *res) //neu username da ton tai thi bao loi, username chua ton tai, cung clienfd thi thay doi username=>xoa username cu
{
    char *username, *password;
    ACCOUNT *acc;

    username = strtok(req->message, "@");
    password = strtok(NULL, "\0");
    printf("xxx%sxxx%sxxx\n", username, password);
    acc = findAccount(username);
    if(acc == NULL){
        res->code = USERNAME_NOT_EXISTED;
        setMessageResponse(res);
        sendRes(clientfd, res, sizeof(Response), 0);
    }else{
        if(strcmp(acc->password, password) == 0){
            if(insertUser(username, clientfd) == NULL){
                res->code = ACCOUNT_BUSY;
                setMessageResponse(res);
                sendRes(clientfd, res, sizeof(Response), 0);
            }else{
                res->code = LOGIN_SUCCESS;
                setMessageResponse(res);
                sendRes(clientfd, res, sizeof(Response), 0);
            }
        }else{
            res->code = WRONG_PASSWORD;
            setMessageResponse(res);
            sendRes(clientfd, res, sizeof(Response), 0);
        }
    }

}

void logOut(int clientfd, Request *req, Response *res){
    deleteUserByClientfd(clientfd);
    res->code = LOGOUT_SUCCESS;
    setMessageResponse(res);
    sendRes(clientfd, res, sizeof(Response), 0);
}

void sendDetail(int clientfd, Request *req, Response *res){
    res->code = RES_DETAIL;
    setMessageResponse(res);
    sendRes(clientfd, res, sizeof(Response), 0);
}


void createRoom(int clientfd, Request *req, Response *res){
    USER *host = findUserByClientfd(clientfd);
    int roomID = countRoom() + 1;
    insertRoom(roomID, host);
    insertPlayer(roomID, host);
    res->code = CREATE_ROOM_SUCCESS;
    setMessageResponse(res);
    sendRes(clientfd, res, sizeof(Response), 0);
}

void quickjoin(int clientfd, Request *req, Response *res){
    USER *user = findUserByClientfd(clientfd);
    char dataString[MAX_LENGTH];
    if(quickJoin(user) == -1){
        res->code = QUICKJOIN_FAIL;
        setMessageResponse(res);
        sendRes(clientfd, res, sizeof(Response), 0);
    }else{
        res->code = QUICKJOIN_SUCCESS;
        sprintf(dataString, "%d", quickJoin(user)); //room ID -> string
        strcpy(res->data, dataString);
        setMessageResponse(res);
        sendRes(clientfd, res, sizeof(Response), 0);
    }
}