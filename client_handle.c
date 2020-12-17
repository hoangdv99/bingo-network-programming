#include "client_handle.h"
#include "protocol.h"
#include "helper.h"
#define BUFF_SIZE 255

void inputRequest(char *sendbuff){
    printf("Please enter the message: ");
    fflush(stdin);
    fgets(sendbuff,BUFF_SIZE,stdin); //enter data
    sendbuff[strlen(sendbuff) - 1] = '\0';
}

int registerAccount(int clientfd, Request *req, Response *res){
    int n_sent = sendReq(clientfd, req, sizeof(Request), 0);
    if (n_sent < 0)
        return n_sent;
    printf("Sent %d bytes to server\n", n_sent);
    printf("Waiting for reply\n");
    int n_recv = recvRes(clientfd, res, sizeof(Response), 0);
    if (n_recv < 0)
        return n_recv;
    printf("Received string with length : %d\n",res->code);
    printf("Received string with length : %s\n",res->message);
    return res->code;
}

int login(int clientfd, char* username, char* password){
    Request *req = (Request *)malloc(sizeof(Request));
    createLoginRequest("LOGIN", username, password, req);
    int n_sent = sendReq(clientfd, req, sizeof(Request), 0);
    if (n_sent < 0)
        return 0;
    printf("Sent %d bytes to server\n", n_sent);
    printf("Waiting for reply\n");
    return 1;
}

int seeDetail(int clientfd, Request *req, Response *res){
    int n_sent = sendReq(clientfd, req, sizeof(Request), 0);
    if (n_sent < 0)
        return n_sent;
    printf("Sent %d bytes to server\n", n_sent);
    printf("Waiting for reply\n");
    int n_recv = recvRes(clientfd, res, sizeof(Response), 0);
    if (n_recv < 0)
        return n_recv;
    printf("Received string with length : %d\n",res->code);
    printf("Received string with length : %s\n",res->message);
    return res->code;
}

int logOut(int clientfd, Request *req, Response *res){
    int n_sent = sendReq(clientfd, req, sizeof(Request), 0);
    if (n_sent < 0)
        return n_sent;
    printf("Sent %d bytes to server\n", n_sent);
    printf("Waiting for reply\n");
    int n_recv = recvRes(clientfd, res, sizeof(Response), 0);
    if (n_recv < 0)
        return n_recv;
    printf("Received string with length : %d\n",res->code);
    printf("Received string with length : %s\n",res->message);
    return res->code;
}

int createRoom(int clientfd, Request *req, Response *res){
    int n_sent = sendReq(clientfd, req, sizeof(Request), 0);
    if (n_sent < 0)
        return n_sent;
    printf("Sent %d bytes to server\n", n_sent);
    printf("Waiting for reply\n");
    int n_recv = recvRes(clientfd, res, sizeof(Response), 0);
    if (n_recv < 0)
        return n_recv;
    printf("Received string with length : %d\n",res->code);
    printf("Received string with length : %s\n",res->message);
    return res->code;
}

int quickJoinClient(int clientfd, Request *req, Response *res){
    int n_sent = sendReq(clientfd, req, sizeof(Request), 0);
    if (n_sent < 0)
        return n_sent;
    printf("Sent %d bytes to server\n", n_sent);
    printf("Waiting for reply\n");
    int n_recv = recvRes(clientfd, res, sizeof(Response), 0);
    if (n_recv < 0)
        return n_recv;
    printf("Received string with length : %d\n",res->code);
    printf("Received string with length : %s\n",res->message);
    return res->code;
}

void createLoginRequest(char *opcode, char *username, char *pass, Request *req){
    char sendbuff[BUFF_SIZE];
    strcpy(sendbuff, opcode);
    strcat(sendbuff, " ");
    strcat(sendbuff, username);
    strcat(sendbuff, "@");
    strcat(sendbuff, pass);
    setOpcodeRequest(req, sendbuff);
}

void createRegisterRequest(char *opcode, char *username, char *pass, char *confirm_pass, Request *req){
    char sendbuff[BUFF_SIZE];
    strcpy(sendbuff, opcode);
    strcat(sendbuff, " ");
    strcat(sendbuff, username);
    strcat(sendbuff, "@");
    strcat(sendbuff, pass);
    strcat(sendbuff, "@");
    strcat(sendbuff, confirm_pass);
    setOpcodeRequest(req, sendbuff);
}
 
void createDetailRequest(char *opcode, Request *req){
    char sendbuff[BUFF_SIZE];
    strcpy(sendbuff, opcode);
    strcat(sendbuff, " ");
    strcat(sendbuff, "detail");
    setOpcodeRequest(req, sendbuff);
}

void createLogOutRequest(char *opcode, Request *req){
    char sendbuff[BUFF_SIZE];
    strcpy(sendbuff, opcode);
    strcat(sendbuff, " ");
    strcat(sendbuff, "logOut");
    setOpcodeRequest(req, sendbuff);
}