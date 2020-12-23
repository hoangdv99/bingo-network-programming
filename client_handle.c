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

int registerAccount(int clientfd, char* username, char* password, char* confirm_pass){
    Request *req = (Request *)malloc(sizeof(Request));
    createRegisterRequest("REGISTER", username, password, confirm_pass, req);
    int n_sent = sendReq(clientfd, req, sizeof(Request), 0);
    if (n_sent < 0)
        return n_sent;
    printf("Sent %d bytes to server\n", n_sent);
    printf("Waiting for reply\n");
    return 1;
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

int logOut(int clientfd, char* username){
    Request *req = (Request *)malloc(sizeof(Request));
    createLogOutRequest("LOGOUT", req, username);
    int n_sent = sendReq(clientfd, req, sizeof(Request), 0);
    if (n_sent < 0)
        return n_sent;
    printf("Sent %d bytes to server\n", n_sent);
    printf("Waiting for reply\n");
    return 1;
}

int createRoom(int clientfd){
    Request *req = (Request *)malloc(sizeof(Request));
    createCreateRoomRequest("CREATE_ROOM", req);
    int n_sent = sendReq(clientfd, req, sizeof(Request), 0);
    if (n_sent < 0)
        return n_sent;
    printf("Sent %d bytes to server\n", n_sent);
    printf("Waiting for reply\n");
    return 1;
}

int quickJoinClient(int clientfd){
    Request *req = (Request *)malloc(sizeof(Request));
    createQuickJoinClientRequest("QUICKJOIN", req);
    int n_sent = sendReq(clientfd, req, sizeof(Request), 0);
    if (n_sent < 0)
        return n_sent;
    printf("Sent %d bytes to server\n", n_sent);
    printf("Waiting for reply\n");
    return 1;
}

int joinClient(int clientfd, char *roomID){
    Request *req = (Request *)malloc(sizeof(Request));
    createJoinClientRequest("JOIN", req, roomID);
    int n_sent = sendReq(clientfd, req, sizeof(Request), 0);
    if (n_sent < 0)
        return n_sent;
    printf("Sent %d bytes to server\n", n_sent);
    printf("Waiting for reply\n");
    return 1;
}

int invite(int clientfd, char* username){
    Request *req = (Request *)malloc(sizeof(Request));
    createInviteRequest("INVITE", req, username);
    int n_sent = sendReq(clientfd, req, sizeof(Request), 0);
    if (n_sent < 0)
        return n_sent;
    printf("Sent %d bytes to server\n", n_sent);
    printf("Waiting for reply\n");
    return 1;
}

/*Create request*/

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

void createLogOutRequest(char *opcode, Request *req, char* username){
    char sendbuff[BUFF_SIZE];
    strcpy(sendbuff, opcode);
    strcat(sendbuff, " ");
    strcat(sendbuff, username);
    setOpcodeRequest(req, sendbuff);
}

void createCreateRoomRequest(char *opcode, Request *req){
    char sendbuff[BUFF_SIZE];
    strcpy(sendbuff, opcode);
    strcat(sendbuff, " ");
    strcat(sendbuff, "create_room");
    setOpcodeRequest(req, sendbuff);
}

void createQuickJoinClientRequest(char *opcode, Request *req){
    char sendbuff[BUFF_SIZE];
    strcpy(sendbuff, opcode);
    strcat(sendbuff, " ");
    strcat(sendbuff, "quick_join");
    setOpcodeRequest(req, sendbuff);
}

void createInviteRequest(char *opcode, Request *req, char *username){
    char sendbuff[BUFF_SIZE];
    strcpy(sendbuff, opcode);
    strcat(sendbuff, " ");
    strcat(sendbuff, username);
    setOpcodeRequest(req, sendbuff);
}

void createJoinClientRequest(char *opcode, Request *req, char *roomID){
    char sendbuff[BUFF_SIZE];
    strcpy(sendbuff, opcode);
    strcat(sendbuff, " ");
    strcat(sendbuff, roomID);
    setOpcodeRequest(req, sendbuff);
}
void splitRoomID(char *input, char *username, char *id){
    int i, usernameLength = 0, idLength = 0;
    for (i = 0; input[i] != '-'; i++){
        username[usernameLength++] = input[i];
    }
    username[usernameLength] = '\0';
    i++;
    for (i; i < strlen(input); i++){
        id[idLength++] = input[i];
    }
    id[idLength] = '\0';
}

void splitPlayerAmountUsernameListRoomID(char *input, char *playerAmount, char *roomID, char *usernameList){
    int i, playerAmountLength = 0, usernameListLength = 0, idLength = 0;
    for (i = 0; input[i] != ' '; i++){
        playerAmount[playerAmountLength++] = input[i];
    }
    playerAmount[playerAmountLength] = '\0';
    i++;
    for (i; input[i] != ' '; i++){
        roomID[idLength++] = input[i];
    }
    roomID[idLength] = '\0';
    i++;
    for (i; i < strlen(input); i++){
        usernameList[usernameListLength++] = input[i];
    }
    usernameList[usernameListLength] = '\0';
}