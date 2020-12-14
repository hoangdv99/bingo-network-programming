#include "handle.h"
#define MAX_PLAYER_IN_ROOM 5

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
    acc = findAccount(username);
    if(acc == NULL){
        res->code = USERNAME_NOT_EXISTED;
        setMessageResponse(res);
        sendRes(clientfd, res, sizeof(Response), 0);
    }else{
        if(strcmp(acc->password, password) == 0){
            USER *user = insertUser(username, clientfd);
            if(user == NULL){
                res->code = ACCOUNT_BUSY;
                setMessageResponse(res);
                sendRes(clientfd, res, sizeof(Response), 0);
            }else{
                printListUser();
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
    deleteUserByUsername(req->message);
    printListUser();
    res->code = LOGOUT_SUCCESS;
    setMessageResponse(res);
    sendRes(clientfd, res, sizeof(Response), 0);
}

void sendDetail(int clientfd, Request *req, Response *res){
    FILE *f = fopen("game_rule.txt", "r");
    char buffer[1024];

	while (fgets(buffer, MAX_LENGTH, f) != NULL)
		strcat(res->message, buffer);
    fclose(f);
    //strcpy(res->message, "When we're playing the game with a number of kids, we play all the variations at once.\nWe have a small prize for each 'combination' (a prize for lines, a prize for T's, a prize for U's, a prize for squares and a prize for Blackouts... if we have more than 5 children, we'll do a separate prize for a vertical line, a diagonal line, a horizontal line, a right-side up U, an upside down U, etc, etc). \nWe have a rule that a person can only win one prize. The game ends when everyone has won something. If two children win the same prize at the same time, we draw straws or 'guess which number I'm thinking' or play 'rock paper scissors' to see who wins the prize and who keeps playing for the next prize. ");
    res->code = RES_DETAIL;
    setMessageResponse(res);
    sendRes(clientfd, res, sizeof(Response), 0);
}


void createRoom(int clientfd, Request *req, Response *res){
    ROOM *newRoom = (ROOM*)malloc(sizeof(ROOM));
    USER *host = findUserByClientfd(clientfd);
    newRoom->host = host;
    newRoom->id = countRoom() + 1;
    newRoom->playerAmount = 0;
    newRoom->status = NOTSTARTED;
    insertRoom(newRoom);
    insertPlayer(newRoom->id, host);
    res->code = CREATE_ROOM_SUCCESS;
    setMessageResponse(res);
    sendRes(clientfd, res, sizeof(Response), 0);
}

void sendInvite(int clientfd, Request *req, Response *res){
    USER *user = findUserByUsername(req->message);
    USER *host = findUserByClientfd(clientfd);

    if(user == NULL || user->status != LOBBY){
        res->code = INVITE_FAIL;
        setMessageResponse(res);
        sendRes(clientfd, res, sizeof(Response), 0);
    }else{
        //send confirm to host
        res->code = INVITE_SUCCESS;
        strcpy(res->data, user->username);
        setMessageResponse(res);
        sendRes(clientfd, res, sizeof(Response), 0);
        //send invitation to user
        // res->code = INVITATION;
        // strcpy(res->data, host->username);
        // setMessageResponse(res);
        // sendRes(user->clientfd, res, sizeof(Response), 0);
    }
}

void kick(int clientfd, Request *req, Response *res){
    if(strcmp(req->message, "") == 0){
        res->code = KICK_FAIL;
        setMessageResponse(res);
        sendRes(clientfd, res, sizeof(Response), 0);
        return;
    }
    ROOM *room = findRoomByClientfd(clientfd);
    USER *user = findUserByUsername(req->message);
    detelePlayerFromRoom(room, user);
    res->code = KICK_SUCCESS;
    strcpy(res->data, req->message);
    setMessageResponse(res);
    sendRes(clientfd, res, sizeof(Response), 0);
    printRoomPlayer(room->id);
}

void quickjoin(int clientfd, Request *req, Response *res){
    USER *user = findUserByClientfd(clientfd);
    char dataString[MAX_LENGTH];
    int roomID = quickJoin(user);
    if(roomID == -1){
        res->code = QUICKJOIN_FAIL;
        setMessageResponse(res);
        sendRes(clientfd, res, sizeof(Response), 0);
    }else{
        insertPlayer(roomID, user);
        res->code = QUICKJOIN_SUCCESS;
        sprintf(dataString, "%d", roomID); //room ID -> string
        strcpy(res->data, dataString);
        setMessageResponse(res);
        sendRes(clientfd, res, sizeof(Response), 0);
    }  
}

void join(int clientfd, Request *req, Response *res){
    int roomID = atoi(req->message);
    ROOM* room = findRoom(roomID);
    USER *user = findUserByClientfd(clientfd);

    if(room == NULL){
        res->code = JOIN_FAIL;
        setMessageResponse(res);
        sendRes(clientfd, res, sizeof(Response), 0);
    }else{
        if(room->playerAmount == MAX_PLAYER_IN_ROOM){
            res->code = ROOM_FULL;
            setMessageResponse(res);
            sendRes(clientfd, res, sizeof(Response), 0);
        }else{
            insertPlayer(roomID, user);
            res->code = JOIN_SUCCESS;
            strcpy(res->data, req->message);
            setMessageResponse(res);
            sendRes(clientfd, res, sizeof(Response), 0);
            printRoomPlayer(roomID);
        }
    }
}

//neu nguoi roi phong la host thi nguoi vao phong som nhat se thanh host va gui thong bao
// cho nguoi do
void outRoom(int clientfd, Request *req, Response *res){
    ROOM *room = findRoomByClientfd(clientfd);
    USER *user = findUserByClientfd(clientfd);

    if(room->host == user){
        room->host = room->player[room->playerAmount - 1];
        res->code = NEW_HOST;
        setMessageResponse(res);
        sendRes(room->player[room->playerAmount - 1]->clientfd, res, sizeof(Response), 0);
    }
    detelePlayerFromRoom(room, user);
    res->code = OUT_ROOM_SUCCESS;
    setMessageResponse(res);
    sendRes(clientfd, res, sizeof(Response), 0);
    printf("Host: %s\n", room->host->username);
}

void exitGame(int clientfd, Request *req, Response *res){
    ROOM *room = findRoomByClientfd(clientfd);
    USER *user = findUserByClientfd(clientfd);

    detelePlayerFromRoom(room, user);
    deleteUserByClientfd(clientfd);
    printRoomPlayer(room->id);
    printListUser();
    res->code = EXIT_GAME_SUCCESS;
    strcpy(res->data, user->username);
    setMessageResponse(res);
    if(room != NULL){
        for(int i = 0; i < room->playerAmount; i++){
            sendRes(room->player[i]->clientfd, res, sizeof(Response), 0);
        }
    }
    
}