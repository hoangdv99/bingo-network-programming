#include "client_handle.h"
#define BUFF_SIZE 255

void inputRequest(char *sendbuff){
    //printf("Please enter the message: ");
    fflush(stdin);
    fgets(sendbuff,BUFF_SIZE,stdin); //enter data
    sendbuff[strlen(sendbuff) - 1] = '\0';
}

void registerAccount(int clientfd ,Request *req, Response *res){
    int n_sent = sendReq(clientfd, req, sizeof(Request), 0);
    // printf("Sent %d bytes to server\n", n_sent);
    // printf("Waiting for reply\n");
    // int n_recv = recvRes(clientfd, res, sizeof(Response), 0);
    // printf("Received string with length : %d\n",res->code);
    // printf("Received string with length : %s\n",res->message);
}

void login(int clientfd ,Request *req, Response *res){
    int n_sent = sendReq(clientfd, req, sizeof(Request), 0);
//     printf("Sent %d bytes to server\n", n_sent);
//     printf("Waiting for reply\n");
//     int n_recv = recvRes(clientfd, res, sizeof(Response), 0);
//     printf("Received string with length : %d\n",res->code);
//     printf("Received string with length : %s\n",res->message);
// 
}

void seeDetail(int clientfd, Request *req, Response *res){
    int n_sent = sendReq(clientfd, req, sizeof(Request), 0);
    // printf("Sent %d bytes to server\n", n_sent);
    // printf("Waiting for reply\n");
    // int n_recv = recvRes(clientfd, res, sizeof(Response), 0);
    // printf("Received string with length : %d\n",res->code);
    // printf("Received string with length : %s\n",res->message);
}

void logOut(int clientfd, Request *req, Response *res){
    int n_sent = sendReq(clientfd, req, sizeof(Request), 0);
    // printf("Sent %d bytes to server\n", n_sent);
    // printf("Waiting for reply\n");
    // int n_recv = recvRes(clientfd, res, sizeof(Response), 0);
    // printf("Received string with length : %d\n",res->code);
    // printf("Received string with length : %s\n",res->message);
}

void createRoom(int clientfd, Request *req, Response *res){
    int n_sent = sendReq(clientfd, req, sizeof(Request), 0);
    // printf("Sent %d bytes to server\n", n_sent);
    // printf("Waiting for reply\n");
    // int n_recv = recvRes(clientfd, res, sizeof(Response), 0);
    // printf("Received string with length : %d\n",res->code);
    // printf("Received string with length : %s\n",res->message);
}

void quickJoinClie(int clientfd, Request *req, Response *res){
    int n_sent = sendReq(clientfd, req, sizeof(Request), 0);
    // printf("Sent %d bytes to server\n", n_sent);
    // printf("Waiting for reply\n");
    // int n_recv = recvRes(clientfd, res, sizeof(Response), 0);
    // printf("Received string with length : %d\n",res->code);
    // printf("Received string with length : %s\n",res->message);
}

void inviteClie(int clientfd, Request *req, Response *res){
    int n_sent = sendReq(clientfd, req, sizeof(Request), 0);
    // printf("Sent %d bytes to server\n", n_sent);
    // printf("Waiting for reply\n");
    // int n_recv = recvRes(clientfd, res, sizeof(Response), 0);
    // printf("Received string with length : %d\n",res->code);
    // printf("Received string with length : %s\n",res->message);
}

void joinClie(int clientfd, Request *req, Response *res){
    int n_sent = sendReq(clientfd, req, sizeof(Request), 0);
    // printf("Sent %d bytes to server\n", n_sent);
    // printf("Waiting for reply\n");
    // int n_recv = recvRes(clientfd, res, sizeof(Response), 0);
    // printf("Received string with length : %d\n",res->code);
    // printf("Received string with length : %s\n",res->message);
}

void outRoomClie(int clientfd, Request *req, Response *res){
    int n_sent = sendReq(clientfd, req, sizeof(Request), 0);
    // printf("Sent %d bytes to server\n", n_sent);
    // printf("Waiting for reply\n");
    // int n_recv = recvRes(clientfd, res, sizeof(Response), 0);
    // printf("Received string with length : %d\n",res->code);
    // printf("Received string with length : %s\n",res->message);
}

void kickClie(int clientfd, Request *req, Response *res){
    int n_sent = sendReq(clientfd, req, sizeof(Request), 0);
    // printf("Sent %d bytes to server\n", n_sent);
    // printf("Waiting for reply\n");
    // int n_recv = recvRes(clientfd, res, sizeof(Response), 0);
    // printf("Received string with length : %d\n",res->code);
    // printf("Received string with length : %s\n",res->message);
}

void acceptInviteClie(int clientfd, Request *req, Response *res){
    int n_sent = sendReq(clientfd, req, sizeof(Request), 0);
    // printf("Sent %d bytes to server\n", n_sent);
    // printf("Waiting for reply\n");
    // int n_recv = recvRes(clientfd, res, sizeof(Response), 0);
    // printf("Received string with length : %d\n",res->code);
    // printf("Received string with length : %s\n",res->message);
}

void exitGameClie(int clientfd, Request *req, Response *res){
    int n_sent = sendReq(clientfd, req, sizeof(Request), 0);
    // printf("Sent %d bytes to server\n", n_sent);
    // printf("Waiting for reply\n");
    // int n_recv = recvRes(clientfd, res, sizeof(Response), 0);
    // printf("Received string with length : %d\n",res->code);
    // printf("Received string with length : %s\n",res->message);
}
void declineInviteClie(int clientfd, Request *req, Response *res){
    int n_sent = sendReq(clientfd, req, sizeof(Request), 0);
    // printf("Sent %d bytes to server\n", n_sent);
    // printf("Waiting for reply\n");
    // int n_recv = recvRes(clientfd, res, sizeof(Response), 0);
    // printf("Received string with length : %d\n",res->code);
    // printf("Received string with length : %s\n",res->message);
}
void playClie(int clientfd, Request *req, Response *res){
    int n_sent = sendReq(clientfd, req, sizeof(Request), 0);
    // printf("Sent %d bytes to server\n", n_sent);
    // printf("Waiting for reply\n");
    // int n_recv = recvRes(clientfd, res, sizeof(Response), 0);
    // printf("Received string with length : %d\n",res->code);
    // printf("Received string with length : %s\n",res->message);
}
void readyClie(int clientfd, Request *req, Response *res){
    int n_sent = sendReq(clientfd, req, sizeof(Request), 0);
}

void unreadyClie(int clientfd, Request *req, Response *res){
    int n_sent = sendReq(clientfd, req, sizeof(Request), 0);
}

void checkReadyClie(int clientfd, Request *req, Response *res){
    int n_sent = sendReq(clientfd, req, sizeof(Request), 0);
}

void bingoClie(int clientfd, Request *req, Response *res){
    int n_sent = sendReq(clientfd, req, sizeof(Request), 0);
}

void pickClie(int clientfd, Request *req, Response *res){
    int n_sent = sendReq(clientfd, req, sizeof(Request), 0);
}

void returnRoomClie(int clientfd, Request *req, Response *res){
    int n_sent = sendReq(clientfd, req, sizeof(Request), 0);
}

void testClie(int clientfd, Request *req, Response *res){
    int n_sent = sendReq(clientfd, req, sizeof(Request), 0);
}
