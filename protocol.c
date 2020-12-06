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
    case RES_DETAIL:
      strcpy(msg->message, "Lorem Ipsum is simply dummy text of the printing and typesetting industry. Lorem Ipsum has been the industry's standard dummy text ever since the 1500s, when an unknown printer took a galley of type and scrambled it to make a type specimen book. It has survived not only five centuries, but also the leap into electronic typesetting, remaining essentially unchanged. It was popularised in the 1960s with the release of Letraset sheets containing Lorem Ipsum passages, and more recently with desktop publishing software like Aldus PageMaker including versions of Lorem Ipsum.");
      break;
    case RES_PLAY:
      break;
    case RES_BACK_TO_MENU:
      break;
    case UNAUTH:
      strcpy(msg->message, "Please login!");
      break;
    case RES_CREATE_ROOM:
      strcpy(msg->message, "New room is created!");
      break;
    case QUICKJOIN_FAIL:
      strcpy(msg->message, "All rooms are full! Please create a new room!");
      break;
    case QUICKJOIN_SUCCESS:
      strcpy(msg->message, "Quick join success!");
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
    case SYNTAX_ERROR:
      printf("%s\n", msg->message);
      break;
    case LOGIN_SUCCESS:
      printf("Welcome %s\n", msg->data);
      break;
    case USERNAME_EXISTED:
      printf("%s\n", msg->message);
      break;
    case UNAUTH:
      printf("%s\n", msg->message);
      break;
    case RES_DETAIL:
      printf("%s\n", msg->message);
      break;
    case RES_PLAY:
      break;
    case RES_BACK_TO_MENU:
      break;
    case RES_CREATE_ROOM:
      printf("%s\n", msg->message);
    case QUICKJOIN_FAIL:
      printf("%s\n", msg->message);
      break;
    case QUICKJOIN_SUCCESS:
      printf("%s\n", msg->message);
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
  else if (strcmp(code, "BACK_TO_MENU") == 0)
    req->code = BACK_TO_MENU;
  else if (strcmp(code, "DETAIL") == 0)
    req->code = DETAIL;
  else if (strcmp(code, "PLAY") == 0)
    req->code = PLAY;
  else if (strcmp(code, "CREATE_ROOM") == 0)
    req->code = CREATE_ROOM;
  else if (strcmp(code, "QUICKJOIN") == 0)
    req->code = QUICKJOIN;
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
