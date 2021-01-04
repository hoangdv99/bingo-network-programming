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

int logOutByX(int clientfd, char* username){
    Request *req = (Request *)malloc(sizeof(Request));
    createLogOutRequest("LOGOUT_BY_X", req, username);
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

int inviteClient(int clientfd, char *username){
    Request *req = (Request *)malloc(sizeof(Request));
    createInviteRequest("INVITE", req, username);
    int n_sent = sendReq(clientfd, req, sizeof(Request), 0);
    if (n_sent < 0)
        return n_sent;
    printf("Sent %d bytes to server\n", n_sent);
    printf("Waiting for reply\n");
    return 1;
}

int acceptInviteClient(int clientfd, char *hostName){
    Request *req = (Request *)malloc(sizeof(Request));
    createAcceptInviteClientRequest("ACCEPT_INVITE_REQUEST", req, hostName);
    int n_sent = sendReq(clientfd, req, sizeof(Request), 0);
    if (n_sent < 0)
        return n_sent;
    printf("Sent %d bytes to server\n", n_sent);
    printf("Waiting for reply\n");
    return 1;
}

int declineInviteClient(int clientfd, char *hostName){
    Request *req = (Request *)malloc(sizeof(Request));
    createDeclineInviteClientRequest("DECLINE_INVITE_REQUEST", req, hostName);
    int n_sent = sendReq(clientfd, req, sizeof(Request), 0);
    if (n_sent < 0)
        return n_sent;
    printf("Sent %d bytes to server\n", n_sent);
    printf("Waiting for reply\n");
    return 1;
}

int backClient(int clientfd, char *username){
    Request *req = (Request *)malloc(sizeof(Request));
    createBackClientRequest("OUT_ROOM", req, username);
    int n_sent = sendReq(clientfd, req, sizeof(Request), 0);
    if (n_sent < 0)
        return n_sent;
    printf("Sent %d bytes to server\n", n_sent);
    printf("Waiting for reply\n");
    return 1;
}

int kickClient(int clientfd, char *username){
    Request *req = (Request *)malloc(sizeof(Request));
    createKickRequest("KICK", req, username);
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

void createLogOutByXRequest(char *opcode, Request *req, char* username){
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

void createAcceptInviteClientRequest(char *opcode, Request *req, char *hostName){
    char sendbuff[BUFF_SIZE];
    strcpy(sendbuff, opcode);
    strcat(sendbuff, " ");
    strcat(sendbuff, hostName);
    setOpcodeRequest(req, sendbuff);
}

void createDeclineInviteClientRequest(char *opcode, Request *req, char *hostName){
    char sendbuff[BUFF_SIZE];
    strcpy(sendbuff, opcode);
    strcat(sendbuff, " ");
    strcat(sendbuff, hostName);
    setOpcodeRequest(req, sendbuff);
}

void createBackClientRequest(char *opcode, Request *req, char *username){
    char sendbuff[BUFF_SIZE];
    strcpy(sendbuff, opcode);
    strcat(sendbuff, " ");
    strcat(sendbuff, username);
    setOpcodeRequest(req, sendbuff);
}

void createKickRequest(char *opcode, Request *req, char *username){
    char sendbuff[BUFF_SIZE];
    strcpy(sendbuff, opcode);
    strcat(sendbuff, " ");
    strcat(sendbuff, username);
    setOpcodeRequest(req, sendbuff);
}

void splitRoomID(char *input, char *username, char *id){
    int i, usernameLength = 0, idLength = 0;
    for (i = 0; input[i] != '-'; i++){
        username[usernameLength++] = input[i];
    }
    username[usernameLength] = '\0';
    i++;
    for (; i < strlen(input); i++){
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
    for (; input[i] != ' '; i++){
        roomID[idLength++] = input[i];
    }
    roomID[idLength] = '\0';
    i++;
    for (; i < strlen(input); i++){
        usernameList[usernameListLength++] = input[i];
    }
    usernameList[usernameListLength] = '\0';
}

void splitHostName(char *input, char *hostName){
    int i, hostNameLength = 0;
    for (i = 0; input[i] != 'm'; i++){//character 'm' from "You receive an invitation from "
        continue;
    }
    i+=2;
    for (; i < strlen(input); i++){
        hostName[hostNameLength++] = input[i];
    }
    hostName[hostNameLength] = '\0';
}

//hoang test
// #include "client_handle.h"
// #define BUFF_SIZE 255

// void inputRequest(char *sendbuff){
//     //printf("Please enter the message: ");
//     fflush(stdin);
//     fgets(sendbuff,BUFF_SIZE,stdin); //enter data
//     sendbuff[strlen(sendbuff) - 1] = '\0';
// }

// void registerAccount(int clientfd ,Request *req, Response *res){
//     int n_sent = sendReq(clientfd, req, sizeof(Request), 0);
//     // printf("Sent %d bytes to server\n", n_sent);
//     // printf("Waiting for reply\n");
//     // int n_recv = recvRes(clientfd, res, sizeof(Response), 0);
//     // printf("Received string with length : %d\n",res->code);
//     // printf("Received string with length : %s\n",res->message);
// }

// void login(int clientfd ,Request *req, Response *res){
//     int n_sent = sendReq(clientfd, req, sizeof(Request), 0);
// //     printf("Sent %d bytes to server\n", n_sent);
// //     printf("Waiting for reply\n");
// //     int n_recv = recvRes(clientfd, res, sizeof(Response), 0);
// //     printf("Received string with length : %d\n",res->code);
// //     printf("Received string with length : %s\n",res->message);
// // 
// }

// void seeDetail(int clientfd, Request *req, Response *res){
//     int n_sent = sendReq(clientfd, req, sizeof(Request), 0);
//     // printf("Sent %d bytes to server\n", n_sent);
//     // printf("Waiting for reply\n");
//     // int n_recv = recvRes(clientfd, res, sizeof(Response), 0);
//     // printf("Received string with length : %d\n",res->code);
//     // printf("Received string with length : %s\n",res->message);
// }

// void logOut(int clientfd, Request *req, Response *res){
//     int n_sent = sendReq(clientfd, req, sizeof(Request), 0);
//     // printf("Sent %d bytes to server\n", n_sent);
//     // printf("Waiting for reply\n");
//     // int n_recv = recvRes(clientfd, res, sizeof(Response), 0);
//     // printf("Received string with length : %d\n",res->code);
//     // printf("Received string with length : %s\n",res->message);
// }

// void createRoom(int clientfd, Request *req, Response *res){
//     int n_sent = sendReq(clientfd, req, sizeof(Request), 0);
//     // printf("Sent %d bytes to server\n", n_sent);
//     // printf("Waiting for reply\n");
//     // int n_recv = recvRes(clientfd, res, sizeof(Response), 0);
//     // printf("Received string with length : %d\n",res->code);
//     // printf("Received string with length : %s\n",res->message);
// }

// void quickJoinClie(int clientfd, Request *req, Response *res){
//     int n_sent = sendReq(clientfd, req, sizeof(Request), 0);
//     // printf("Sent %d bytes to server\n", n_sent);
//     // printf("Waiting for reply\n");
//     // int n_recv = recvRes(clientfd, res, sizeof(Response), 0);
//     // printf("Received string with length : %d\n",res->code);
//     // printf("Received string with length : %s\n",res->message);
// }

// void inviteClie(int clientfd, Request *req, Response *res){
//     int n_sent = sendReq(clientfd, req, sizeof(Request), 0);
//     // printf("Sent %d bytes to server\n", n_sent);
//     // printf("Waiting for reply\n");
//     // int n_recv = recvRes(clientfd, res, sizeof(Response), 0);
//     // printf("Received string with length : %d\n",res->code);
//     // printf("Received string with length : %s\n",res->message);
// }

// void joinClie(int clientfd, Request *req, Response *res){
//     int n_sent = sendReq(clientfd, req, sizeof(Request), 0);
//     // printf("Sent %d bytes to server\n", n_sent);
//     // printf("Waiting for reply\n");
//     // int n_recv = recvRes(clientfd, res, sizeof(Response), 0);
//     // printf("Received string with length : %d\n",res->code);
//     // printf("Received string with length : %s\n",res->message);
// }

// void outRoomClie(int clientfd, Request *req, Response *res){
//     int n_sent = sendReq(clientfd, req, sizeof(Request), 0);
//     // printf("Sent %d bytes to server\n", n_sent);
//     // printf("Waiting for reply\n");
//     // int n_recv = recvRes(clientfd, res, sizeof(Response), 0);
//     // printf("Received string with length : %d\n",res->code);
//     // printf("Received string with length : %s\n",res->message);
// }

// void kickClie(int clientfd, Request *req, Response *res){
//     int n_sent = sendReq(clientfd, req, sizeof(Request), 0);
//     // printf("Sent %d bytes to server\n", n_sent);
//     // printf("Waiting for reply\n");
//     // int n_recv = recvRes(clientfd, res, sizeof(Response), 0);
//     // printf("Received string with length : %d\n",res->code);
//     // printf("Received string with length : %s\n",res->message);
// }

// void acceptInviteClie(int clientfd, Request *req, Response *res){
//     int n_sent = sendReq(clientfd, req, sizeof(Request), 0);
//     // printf("Sent %d bytes to server\n", n_sent);
//     // printf("Waiting for reply\n");
//     // int n_recv = recvRes(clientfd, res, sizeof(Response), 0);
//     // printf("Received string with length : %d\n",res->code);
//     // printf("Received string with length : %s\n",res->message);
// }

// void closeGameClie(int clientfd, Request *req, Response *res){
//     int n_sent = sendReq(clientfd, req, sizeof(Request), 0);
//     // printf("Sent %d bytes to server\n", n_sent);
//     // printf("Waiting for reply\n");
//     // int n_recv = recvRes(clientfd, res, sizeof(Response), 0);
//     // printf("Received string with length : %d\n",res->code);
//     // printf("Received string with length : %s\n",res->message);
// }
// void declineInviteClie(int clientfd, Request *req, Response *res){
//     int n_sent = sendReq(clientfd, req, sizeof(Request), 0);
//     // printf("Sent %d bytes to server\n", n_sent);
//     // printf("Waiting for reply\n");
//     // int n_recv = recvRes(clientfd, res, sizeof(Response), 0);
//     // printf("Received string with length : %d\n",res->code);
//     // printf("Received string with length : %s\n",res->message);
// }
// void playClie(int clientfd, Request *req, Response *res){
//     int n_sent = sendReq(clientfd, req, sizeof(Request), 0);
//     // printf("Sent %d bytes to server\n", n_sent);
//     // printf("Waiting for reply\n");
//     // int n_recv = recvRes(clientfd, res, sizeof(Response), 0);
//     // printf("Received string with length : %d\n",res->code);
//     // printf("Received string with length : %s\n",res->message);
// }
// void readyClie(int clientfd, Request *req, Response *res){
//     int n_sent = sendReq(clientfd, req, sizeof(Request), 0);
// }

// void unreadyClie(int clientfd, Request *req, Response *res){
//     int n_sent = sendReq(clientfd, req, sizeof(Request), 0);
// }

// void checkReadyClie(int clientfd, Request *req, Response *res){
//     int n_sent = sendReq(clientfd, req, sizeof(Request), 0);
// }

// void bingoClie(int clientfd, Request *req, Response *res){
//     int n_sent = sendReq(clientfd, req, sizeof(Request), 0);
// }

// void pickClie(int clientfd, Request *req, Response *res){
//     int n_sent = sendReq(clientfd, req, sizeof(Request), 0);
// }

// void returnRoomClie(int clientfd, Request *req, Response *res){
//     int n_sent = sendReq(clientfd, req, sizeof(Request), 0);
// }

// void testClie(int clientfd, Request *req, Response *res){
//     int n_sent = sendReq(clientfd, req, sizeof(Request), 0);
// }
// void testClie(int clientfd, Request *req, Response *res){
//     int n_sent = sendReq(clientfd, req, sizeof(Request), 0);
// }

// void quitGameClie(int clientfd, Request *req, Response *res){
//     int n_sent = sendReq(clientfd, req, sizeof(Request), 0);
// }
