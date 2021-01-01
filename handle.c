#include "handle.h"
#define MAX_PLAYER_IN_ROOM 5
#define MAX_STRING 256
extern ACCOUNT *accountListHead;
extern USER *userListHead;
extern ROOM *roomListHead;

void signUp(int clientfd, Request *req, Response *res)
{
    char *username, *password, *confirmPassword;

    username = strtok(req->message, "@");
    password = strtok(NULL, "@");
    confirmPassword = strtok(NULL, "\0");

    if (strcmp(username, "") == 0 || strcmp(password, "") == 0 || strcmp(confirmPassword, "") == 0 || strcmp(password, confirmPassword) != 0)
    {
        res->code = REGISTER_INPUT_WRONG;
        setMessageResponse(res);
        sendRes(clientfd, res, sizeof(Response), 0);
        return;
    }
    if ((findAccount(username)) != NULL)
    {
        res->code = USERNAME_EXISTED;
        setMessageResponse(res);
        sendRes(clientfd, res, sizeof(Response), 0);
        return;
    }
    insertAcc(username, password, 1);
    writeToAccountFile();
    insertUser(username, clientfd);
    res->code = REGISTER_SUCCESS;
    setMessageResponse(res);
    sendRes(clientfd, res, sizeof(Response), 0);
}

void logIn(int clientfd, Request *req, Response *res) //neu username da ton tai thi bao loi, username chua ton tai, cung clienfd thi thay doi username=>xoa username cu
{
    char *username, *password;
    ACCOUNT *acc;

    username = strtok(req->message, "@");
    password = strtok(NULL, "\0");
    acc = findAccount(username);
    if (acc == NULL)
    {
        res->code = USERNAME_NOT_EXISTED;
        setMessageResponse(res);
        sendRes(clientfd, res, sizeof(Response), 0);
    }
    else
    {
        if (strcmp(acc->password, password) == 0)
        {
            USER *user = insertUser(username, clientfd);
            if (user == NULL)
            {
                res->code = ACCOUNT_BUSY;
                setMessageResponse(res);
                sendRes(clientfd, res, sizeof(Response), 0);
            }
            else
            {
                printListUser();
                res->code = LOGIN_SUCCESS;
                setMessageResponse(res);
                sendRes(clientfd, res, sizeof(Response), 0);
            }
        }
        else
        {
            res->code = WRONG_PASSWORD;
            setMessageResponse(res);
            sendRes(clientfd, res, sizeof(Response), 0);
        }
    }
}

void logOut(int clientfd, Request *req, Response *res)
{
    deleteUserByUsername(req->message);
    printListUser();
    res->code = LOGOUT_SUCCESS;
    setMessageResponse(res);
    sendRes(clientfd, res, sizeof(Response), 0);
}

void logOutByX(int clientfd, Request *req, Response *res)
{
    ROOM *room = findRoomByClientfd(clientfd);
    USER *user = findUserByUsername(req->message);
    detelePlayerFromRoom(room, user);

    if (room->host == user)
    {
        if (room->playerAmount == 1){//phong chi co moi host thi xoa luon phong
            room->playerAmount-=1;
            res->code = LOGOUT_SUCCESS;
            setMessageResponse(res);
            sendRes(clientfd, res, sizeof(Response), 0);
            detelePlayerFromRoom(room, user);
            deleteRoom(room->id);
            deleteUserByUsername(req->message);
            printListUser();
            return;
        }
        detelePlayerFromRoom(room, user);//phong co nhieu hon 1 nguoi chi chuyen quyen host cho nguoi dau
        deleteUserByUsername(req->message);
        printListUser();
        room->host = room->player[0];
        res->code = NEW_HOST;
        setMessageResponse(res);
        sendRes(room->player[0]->clientfd, res, sizeof(Response), 0);
    }
    detelePlayerFromRoom(room, user);
    deleteUserByUsername(req->message);
    printListUser();
    res->code = LOGOUT_SUCCESS;
    setMessageResponse(res);
    sendRes(clientfd, res, sizeof(Response), 0);
    //Send to other clients in room
    char buffer[MAX_STRING];
    sprintf(buffer, "%d", room->playerAmount); //Format: playerAmount roomID username1-username2
    strcat(buffer, " ");
    char id[3];
    sprintf(id, "%d", room->id);
    strcat(buffer, id);
    strcat(buffer, " ");
    for (int i = 0; i < room->playerAmount; i++)
    {
        strcat(buffer, room->player[i]->username);
        if (i == room->playerAmount - 1)
            break;
        else
        {
            strcat(buffer, "-");
        }
    }
    for (int i = 0; i < room->playerAmount; i++){
        res->code = ROOM_CHANGED;
        strcpy(res->data, buffer);
        setMessageResponse(res);
        sendRes(room->player[i]->clientfd, res, sizeof(Response), 0);
    }
}

void sendDetail(int clientfd, Request *req, Response *res)
{
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

void createRoom(int clientfd, Request *req, Response *res)
{
    ROOM *newRoom = (ROOM *)malloc(sizeof(ROOM));
    USER *host = findUserByClientfd(clientfd);
    newRoom->host = host;
    newRoom->id = countRoom() + 1;
    printf("\nRoom ID: %d\n", newRoom->id);
    newRoom->playerAmount = 0;
    newRoom->status = NOTSTARTED;
    insertRoom(newRoom);
    insertPlayer(newRoom->id, host);
    res->code = CREATE_ROOM_SUCCESS;
    strcpy(res->data, host->username);
    strcat(res->data, "-");
    char buffer[33];
    sprintf(buffer, "%d", newRoom->id);
    strcat(res->data, buffer);
    setMessageResponse(res);
    sendRes(clientfd, res, sizeof(Response), 0);
}

void sendInvite(int clientfd, Request *req, Response *res)
{
    USER *user = findUserByUsername(req->message);
    USER *host = findUserByClientfd(clientfd);

    if (user == NULL || user->status != LOBBY)
    {
        res->code = INVITE_FAIL;
        setMessageResponse(res);
        sendRes(clientfd, res, sizeof(Response), 0);
    }
    else
    {
        //send confirm to host
        res->code = INVITE_SUCCESS;
        strcpy(res->data, user->username);
        setMessageResponse(res);
        sendRes(clientfd, res, sizeof(Response), 0);
        //send invitation to user
        res->code = INVITATION;
        strcpy(res->data, host->username);
        setMessageResponse(res);
        sendRes(user->clientfd, res, sizeof(Response), 0);
    }
}

void kick(int clientfd, Request *req, Response *res)
{
    if (strcmp(req->message, "") == 0)
    {
        res->code = KICK_FAIL;
        setMessageResponse(res);
        sendRes(clientfd, res, sizeof(Response), 0);
        return;
    }
    ROOM *room = findRoomByClientfd(clientfd);
    USER *user = findUserByUsername(req->message);
    detelePlayerFromRoom(room, user);

    char buffer[MAX_STRING];
    sprintf(buffer, "%d", room->playerAmount); //Format: playerAmount roomID username1-username2
    strcat(buffer, " ");
    char id[3];
    sprintf(id, "%d", room->id);
    strcat(buffer, id);
    strcat(buffer, " ");
    for (int i = 0; i < room->playerAmount; i++)
    {
        strcat(buffer, room->player[i]->username);
        if (i == room->playerAmount - 1)
            break;
        else
        {
            strcat(buffer, "-");
        }
    }
    res->code = KICK_SUCCESS;
    strcpy(res->data, user->username);
    setMessageResponse(res);
    sendRes(clientfd, res, sizeof(Response), 0);

    res->code = BE_KICKED;
    strcpy(res->data, user->username);
    setMessageResponse(res);
    sendRes(user->clientfd, res, sizeof(Response), 0);

    res->code = ROOM_CHANGED;
    strcpy(res->data, buffer);
    setMessageResponse(res);

    for (int i = 0; i < room->playerAmount; i++)
    {
        sendRes(room->player[i]->clientfd, res, sizeof(Response), 0);
    }
    printf("\n%s", res->message);
}

void quickjoin(int clientfd, Request *req, Response *res)
{
    USER *user = findUserByClientfd(clientfd);
    int roomID = quickJoin(user);
    ROOM *room = findRoom(roomID);
    char buffer[MAX_STRING];
    if (roomID == -1)
    {
        res->code = QUICKJOIN_FAIL;
        setMessageResponse(res);
        sendRes(clientfd, res, sizeof(Response), 0);
    }
    else
    {
        sprintf(buffer, "%d", room->playerAmount); //Format: playerAmount roomID username1-username2
        strcat(buffer, " ");
        char id[3];
        sprintf(id, "%d", roomID);
        strcat(buffer, id);
        strcat(buffer, " ");
        for (int i = 0; i < room->playerAmount; i++)
        {
            strcat(buffer, room->player[i]->username);
            if (i == room->playerAmount - 1)
                break;
            else
            {
                strcat(buffer, "-");
            }
        }
        for (int i = 0; i < room->playerAmount; i++)
        {
            if (room->player[i]->clientfd == clientfd)
            {
                res->code = QUICKJOIN_SUCCESS;
                strcpy(res->data, buffer);
                setMessageResponse(res);
                sendRes(room->player[i]->clientfd, res, sizeof(Response), 0);
            }
            else
            {
                res->code = ROOM_CHANGED;
                strcpy(res->data, buffer);
                setMessageResponse(res);
                sendRes(room->player[i]->clientfd, res, sizeof(Response), 0);
            }
        }
    }
}

void join(int clientfd, Request *req, Response *res)
{
    int roomID = atoi(req->message);
    ROOM *room = findRoom(roomID);
    USER *user = findUserByClientfd(clientfd);
    char buffer[MAX_STRING];
    
    if (room == NULL || room->status == STARTED)
    {
        res->code = JOIN_FAIL;
        setMessageResponse(res);
        sendRes(clientfd, res, sizeof(Response), 0);
    }
    else
    {
        if (room->playerAmount == MAX_PLAYER_IN_ROOM)
        {
            res->code = ROOM_FULL;
            setMessageResponse(res);
            sendRes(clientfd, res, sizeof(Response), 0);
        }
        else
        {
            insertPlayer(roomID, user);
            printRoomPlayer(roomID);
            sprintf(buffer, "%d", room->playerAmount); //Format: playerAmount roomID username1-username2
            strcat(buffer, " ");
            char id[3];
            sprintf(id, "%d", roomID);
            strcat(buffer, id);
            strcat(buffer, " ");
            for (int i = 0; i < room->playerAmount; i++)
            {
                strcat(buffer, room->player[i]->username);
                if (i == room->playerAmount - 1)
                    break;
                else
                {
                    strcat(buffer, "-");
                }
            }
            for (int i = 0; i < room->playerAmount; i++)
            {
                if (room->player[i]->clientfd == clientfd)
                {
                    res->code = JOIN_SUCCESS;
                    strcpy(res->data, buffer);
                    setMessageResponse(res);
                    sendRes(room->player[i]->clientfd, res, sizeof(Response), 0);
                }
                else
                {
                    res->code = ROOM_CHANGED;
                    strcpy(res->data, buffer);
                    setMessageResponse(res);
                    sendRes(room->player[i]->clientfd, res, sizeof(Response), 0);
                }
            }
        }
    }
    
}

//neu nguoi roi phong la host thi nguoi vao phong som nhat se thanh host va gui thong bao
// cho nguoi do
void outRoom(int clientfd, Request *req, Response *res)
{
    ROOM *room = findRoomByClientfd(clientfd);
    USER *user = findUserByClientfd(clientfd);
    char buffer[MAX_STRING];

    if (room->host == user)
    {
        if (room->playerAmount == 1){//phong chi co moi host thi xoa luon phong
            room->playerAmount-=1;
            res->code = OUT_ROOM_SUCCESS;
            setMessageResponse(res);
            sendRes(clientfd, res, sizeof(Response), 0);
            detelePlayerFromRoom(room, user);
            deleteRoom(room->id);
            return;
        }
        detelePlayerFromRoom(room, user);//phong co nhieu hon 1 nguoi chi chuyen quyen host cho nguoi dau
        room->host = room->player[0];
        res->code = NEW_HOST;
        setMessageResponse(res);
        sendRes(room->player[0]->clientfd, res, sizeof(Response), 0);
    }
    detelePlayerFromRoom(room, user);
    sprintf(buffer, "%d", room->playerAmount); //Format: playerAmount roomID username1-username2
    strcat(buffer, " ");
    char id[3];
    sprintf(id, "%d", room->id);
    strcat(buffer, id);
    strcat(buffer, " ");
    for (int i = 0; i < room->playerAmount; i++)
    {
        strcat(buffer, room->player[i]->username);
        if (i == room->playerAmount - 1)
            break;
        else
        {
            strcat(buffer, "-");
        }
    }
    res->code = OUT_ROOM_SUCCESS;
    strcpy(res->data, user->username);
    setMessageResponse(res);
    sendRes(clientfd, res, sizeof(Response), 0);
    for (int i = 0; i < room->playerAmount; i++)
    {
        res->code = ROOM_CHANGED;
        strcpy(res->data, buffer);
        setMessageResponse(res);
        sendRes(room->player[i]->clientfd, res, sizeof(Response), 0);
    }
    // printf("Host: %s\n", room->host->username);
    printRoomPlayer(room->id);
}

void closeGame(int clientfd, Request *req, Response *res)
{
    ROOM *room = findRoomByClientfd(clientfd);

    if(room != NULL){
        outRoom(clientfd, req, res);
    }
    deleteUserByClientfd(clientfd);
}

//ACCEPT_INVITE hoang
void acceptInvite(int clientfd, Request *req, Response *res)
{
    USER *user = findUserByClientfd(clientfd);
    USER *host = findUserByUsername(req->message);
    ROOM *room = findRoomByClientfd(host->clientfd);
    char buffer[MAX_STRING];
    insertPlayer(room->id, user);

    sprintf(buffer, "%d", room->playerAmount); //Format: playerAmount roomID username1-username2
    strcat(buffer, " ");
    char id[3];
    sprintf(id, "%d", room->id);
    strcat(buffer, id);
    strcat(buffer, " ");
    for (int i = 0; i < room->playerAmount; i++)
    {
        strcat(buffer, room->player[i]->username);
        if (i == room->playerAmount - 1)
            break;
        else
        {
            strcat(buffer, "-");
        }
    }
    for (int i = 0; i < room->playerAmount; i++)
    {
        if (room->player[i]->clientfd == clientfd)
        {
            res->code = ACCEPTED;
            strcpy(res->data, buffer);
            setMessageResponse(res);
            sendRes(room->player[i]->clientfd, res, sizeof(Response), 0);
        }
        else
        {
            res->code = ROOM_CHANGED;
            strcpy(res->data, buffer);
            setMessageResponse(res);
            sendRes(room->player[i]->clientfd, res, sizeof(Response), 0);
        }
    }
}
//DECLINE_INVITE hoang
void declineInvite(int clientfd, Request *req, Response *res)
{
    USER *user = findUserByClientfd(clientfd);
    USER *host = findUserByUsername(req->message);
    res->code = DECLINED;
    strcpy(res->data, user->username);
    setMessageResponse(res);
    sendRes(host->clientfd, res, sizeof(Response), 0);
}

void play(THREAD_DATA threadData, int clientfd, Request *req, Response *res)
{
    ROOM *room = findRoomByClientfd(clientfd);
    room->status = STARTED;
    pthread_t tid;
    pthread_create(&tid, NULL, roomThreadFunc, &threadData);
    res->code = GAME_START;
    setMessageResponse(res);
    for (int i = 0; i < room->playerAmount; i++)
    {
        room->player[i]->status = INGAME;
        sendRes(room->player[i]->clientfd, res, sizeof(Response), 0);
    }
}

void ready(int clientfd, Request *req, Response *res)
{
    USER *user = findUserByClientfd(clientfd);
    ROOM *room = findRoomByClientfd(clientfd);

    user->status = READY;
    res->code = READY_SUCCESS;
    strcpy(res->data, user->username);
    setMessageResponse(res);
    sendRes(clientfd, res, sizeof(Response), 0);
    sendRes(room->host->clientfd, res, sizeof(Response), 0);
}

void unready(int clientfd, Request *req, Response *res)
{
    USER *user = findUserByClientfd(clientfd);
    ROOM *room = findRoomByClientfd(clientfd);

    user->status = INROOM;
    res->code = UNREADY_SUCCESS;
    strcpy(res->data, user->username);
    setMessageResponse(res);
    sendRes(clientfd, res, sizeof(Response), 0);
    sendRes(room->host->clientfd, res, sizeof(Response), 0);
}

int checkReady(int clientfd)
{
    ROOM *room = findRoomByClientfd(clientfd);
    for (int i = 0; i < room->playerAmount; i++)
    {
        if (room->player[i] != room->host)
        {
            if (room->player[i]->status != READY)
            {
                // res->code = SOMEONE_UNREADY;
                // setMessageResponse(res);
                // sendRes(room->host->clientfd, res, sizeof(Response), 0);
                return 0;
            }
        }
    }
    return 1;
}

void startGame(int sockfd, int clientfd, Request *req, Response *res)
{
    THREAD_DATA threadData;
    ROOM *room = findRoomByClientfd(clientfd);
    if(room->playerAmount == 1){
        res->code = PLAYER_NOT_ENOUGH;
        setMessageResponse(res);
        sendRes(clientfd, res, sizeof(Response), 0);
        
    }else if(checkReady(clientfd) == 0){
        res->code = SOMEONE_UNREADY;
        setMessageResponse(res);
        sendRes(clientfd, res, sizeof(Response), 0);
    }
    else
    {
        threadData.clientfd = clientfd;
        threadData.master_socket = sockfd;
        play(threadData, clientfd, req, res);
    }
}

void returnRoom(int clientfd, Request *req, Response *res){
    ROOM *room = findRoomByClientfd(clientfd);
    res->code = RETURN_ROOM_SUCCESS;
    setMessageResponse(res);
    for (int i = 0; i < room->playerAmount; i++)
    {
        room->player[i]->status = INROOM;
        sendRes(room->player[i]->clientfd, res, sizeof(Response), 0);
    }
}
