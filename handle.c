#include "handle.h"

void login(int socket ,Request *req, Response *res)
{
    char username[MAX_LENGTH];
    
    strcpy(username, req->message);
    if ((findUser(username)) != NULL)
    {
        res->code = USERNAME_EXISTED;
        setMessageResponse(res);
    }else
    {
        res->code = LOGIN_SUCCESS;
        setMessageResponse(res);
        insertUser(username, socket);
    }
    printListUser();
    sendRes(socket, res, sizeof(Response), 0);

}