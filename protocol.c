#include "protocol.h"
#define BUFF_SIZE 255

int recvReq(int socket, Request *buff, int size, int flags){
  int n;

  n = recv(socket, buff, size, flags);
  if (n < 0)
    perror("Error: ");
  return n;
}

int sendReq(int socket, Request *buff, int size, int flags)
{
  int n;

  n = send(socket, buff, size, flags);
  if (n < 0)
    perror("Error: ");
  return n;
}

int sendRes(int socket, Response *msg, int size, int flags)
{
  int n;
  n = send(socket, msg, size, flags);
  if (n < 0)
    perror("Error: ");
  return n;
}

int recvRes(int socket, Response *msg, int size, int flags)
{
  int n;
  n = recv(socket, msg, size, flags);
  if (n < 0)
    perror("Error: ");
  return n;
}

void setMessageResponse(Response *msg){
  if(msg->code != NULL){
    switch (msg->code)
    {
    case SYNTAX_ERROR:
        strcpy(msg->message, "Syntax error");
      break;
    case USERNAME_EXISTED:
      strcpy(msg->message, "Username existed! Please choose another!");
      break;
    case LOGIN_SUCCESS:
      strcpy(msg->message, "Login successfully!");
      break;
    default:
      break;
    }
  }
}

void readMessageReponse(Response *msg){
  if(msg->code != NULL){
    printf("%s\n", msg->message);
    switch (msg->code)
    {
    case LOGIN_SUCCESS:
      printf("Welcome %s\n", msg->data);
      break;
    
    default:
      break;
    }
  }
}

void setOpcodeRequest(Request *req, char *input){
  char code[BUFF_SIZE], data[BUFF_SIZE];

  splitMessage(input, code, data);
  strcpy(req->message, data);
  if (strcmp(code, "LOGIN") == 0)
    req->code = LOGIN;
  else if (strcmp(code, "BACKTOMENU") == 0)
    req->code = BACKTOMENU;
}

int sendNum(int socket, int num, int size, int flags)
{
  int n;
  n = send(socket, num, size, flags);
  if (n < 0)
    perror("Error: ");
  return n;
}

int recvNum(int socket, int num, int size, int flags)
{
  int n;
  n = recv(socket, num, size, flags);
  if (n < 0)
    perror("Error: ");
  return n;
}
