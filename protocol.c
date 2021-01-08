#include <sys/socket.h>
#include "protocol.h"
#include "helper.h"
#define REMAIN_TIME 30

int recvReq(int socket, Request *buff, int size, int flags)
{
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

void setMessageResponse(Response *msg)
{
  if (msg->code != -1)
  {
    switch (msg->code)
    {
    case ROOM_CHANGED:
      strcpy(msg->message, "The number of player has changed!");
      break;
    case SYNTAX_ERROR:
      strcpy(msg->message, "Syntax error");
      break;
    case REGISTER_INPUT_WRONG:
      strcpy(msg->message, "Wrong input. Try again!");
      break;
    case USERNAME_EXISTED:
      strcpy(msg->message, "Username existed! Please choose another!");
      break;
    case REGISTER_SUCCESS:
      strcpy(msg->message, "Register successfully!");
      break;
    case USERNAME_NOT_EXISTED:
      strcpy(msg->message, "This account is not registered!");
      break;
    case ACCOUNT_BUSY:
      strcpy(msg->message, "This account is using by other player!");
      break;
    case RES_DETAIL:
      break;
    case LOGIN_SUCCESS:
      strcpy(msg->message, "Login successfully!");
      break;
    case WRONG_PASSWORD:
      strcpy(msg->message, "Wrong password!");
      break;
    case LOGOUT_SUCCESS:
      strcpy(msg->message, "Logout successfully!");
      break;
    case CREATE_ROOM_SUCCESS:
      strcpy(msg->message, "New room is created!");
      break;
    case INVITATION:
      strcpy(msg->message, "You receive an invitation from ");
      strcat(msg->message, msg->data);
      break;
    case INVITE_SUCCESS:
      strcpy(msg->message, "Sent invitation to ");
      strcat(msg->message, msg->data);
      break;
    case INVITE_FAIL:
      strcpy(msg->message, "Cannot invite this player!");
      break;
    case ACCEPTED:
      strcpy(msg->message, "You accepted!");
      break;
    case DECLINED:
      strcpy(msg->message, "");
      strcat(msg->data, " declined your invitation!");
      strcpy(msg->message, msg->data);
      break;
    case QUICKJOIN_FAIL:
      strcpy(msg->message, "All rooms are full! Please create a new room!");
      break;
    case QUICKJOIN_SUCCESS:
      strcpy(msg->message, "You entered room!");
      break;
    case JOIN_SUCCESS:
      strcpy(msg->message, "Join success!");
      break;
    case JOIN_FAIL:
      strcpy(msg->message, "This room is not existing!");
      break;
    case ROOM_FULL:
      strcpy(msg->message, "This room is full. Please join another!");
      break;
    case NEW_HOST:
      strcpy(msg->message, "You have become room's host. Now you can invite or kick other players.");
      break;
    case OUT_ROOM_SUCCESS:
      strcpy(msg->message, "Out room successfully!");
      break;
    case KICK_SUCCESS:
      strcat(msg->data, " was kicked!");
      strcpy(msg->message, "");
      strcat(msg->message, msg->data);
      break;
    case KICK_FAIL:
      strcpy(msg->message, "Kick failed!");
      break;
    case BE_KICKED:
      strcpy(msg->message, "You are be kicked!");
      break;
    case GAME_START:
      strcpy(msg->message, "Game started! Each turn has a duration of 30 seconds..");
      break;
    case YOUR_TURN:
      strcpy(msg->message, "Your turn!");
      break;
    case BOARD_DATA_GENERATED:
      //strcat("Board data: ", msg->data);
      strcpy(msg->message, msg->data);
      break;
    case OTHER_PLAYER_TURN:
      strcat(msg->data, " 's turn!");
      strcpy(msg->message, msg->data);
      break;
    case BINGO_REAL:
      strcpy(msg->message, "Bingo yoooooooo!!");
      break;
    case BINGO_FAKE:
      strcpy(msg->message, "Do not lie!");
      break;
    case PICK_FAIL:
      strcpy(msg->message, "Wrong number. Please pick another!");
      break;
    case PICK_SUCCESS:
      strcpy(msg->message, msg->data);
      strcat(msg->message, " is picked");
      break;
    case YOU_WIN:
      strcpy(msg->message, "You won!");
      break;
    case OTHER_PLAYER_WIN:
      strcat(msg->data, " won!");
      strcpy(msg->message, msg->data);
      break;
    case NO_ROOM:
      strcpy(msg->message, "No room has been created! Please create a new room!");
      break;
    case READY_SUCCESS:
      break;
    case UNREADY_SUCCESS:
      strcpy(msg->message, "Unready successfully!");
      break;
    case ALL_PLAYERS_READY:
      strcpy(msg->message, "All players are ready!");
      break;
    case SOMEONE_UNREADY:
      strcpy(msg->message, "Someone is not ready!");
      break;
    case PLAYER_NOT_ENOUGH:
      strcpy(msg->message, "Not enough players! Invite someone!");
      break;
    case NEW_PLAYER_JOINED:
      strcpy(msg->message, msg->data);
      break;
    case SOMEONE_LEFT_GAME:
      strcat(msg->data, " has left the game!");
      strcpy(msg->message, msg->data);
      break;
    case ALL_PLAYERS_LEFT_GAME:
      strcpy(msg->message, "All players have left the game. Game ends!");
      break;
    case RETURN_ROOM_SUCCESS:
      strcpy(msg->message, "Return room success!");
      break;
    case DISCONNECTED:
      strcpy(msg->message, "You have been disconnected!");
      break;
    default:
      break;
    }
  }
}

void readMessageReponse(Response *msg)
{
  if (msg->code != -1)
  {
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
    case RES_DETAIL:
      printf("%s\n", msg->message);
      break;
    case CREATE_ROOM_SUCCESS:
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

void setOpcodeRequest(Request *req, char *input)
{
  char code[MAX_LENGTH], data[MAX_LENGTH];

  splitMessage(input, code, data);
  printf("\n%s-%s\n", code, data);
  strcpy(req->message, data);
  if (strcmp(code, "REGISTER") == 0)
    req->code = REGISTER;
  else if (strcmp(code, "LOGIN") == 0)
    req->code = LOGIN;
  else if (strcmp(code, "DETAIL") == 0)
    req->code = DETAIL;
  else if (strcmp(code, "LOGOUT") == 0)
    req->code = LOGOUT;
  else if (strcmp(code, "CREATE_ROOM") == 0)
    req->code = CREATE_ROOM;
  else if (strcmp(code, "QUICKJOIN") == 0)
    req->code = QUICKJOIN;
  else if (strcmp(code, "LOGOUT") == 0)
    req->code = LOGOUT;
  else if (strcmp(code, "INVITE") == 0)
    req->code = INVITE;
  else if (strcmp(code, "KICK") == 0)
    req->code = KICK;
  else if (strcmp(code, "JOIN") == 0)
    req->code = JOIN;
  else if (strcmp(code, "OUT_ROOM") == 0)
    req->code = OUT_ROOM;
  else if (strcmp(code, "ACCEPT_INVITE") == 0)
    req->code = ACCEPT_INVITE;
  else if (strcmp(code, "DECLINE_INVITE") == 0)
    req->code = DECLINE_INVITE;
  else if (strcmp(code, "CLOSE") == 0)
    req->code = CLOSE;
  else if (strcmp(code, "PLAY") == 0)
    req->code = PLAY;
  else if (strcmp(code, "BINGO") == 0)
    req->code = BINGO;
  else if (strcmp(code, "PICK") == 0)
    req->code = PICK;
  else if (strcmp(code, "READY1") == 0)
    req->code = READY1;
  else if (strcmp(code, "UNREADY") == 0)
    req->code = UNREADY;
  else if (strcmp(code, "CHECK_READY") == 0)
    req->code = CHECK_READY;
  else if (strcmp(code, "RETURN_ROOM") == 0)
    req->code = RETURN_ROOM;
  else if (strcmp(code, "QUIT") == 0)
    req->code = QUIT;
  else if (strcmp(code, "TEST") == 0)
    req->code = TEST;
  else if (strcmp(code, "ACCEPT_INVITE_REQUEST") == 0)
    req->code = ACCEPT_INVITE_REQUEST;
  else if (strcmp(code, "DECLINE_INVITE_REQUEST") == 0)
    req->code = DECLINE_INVITE_REQUEST;
  else{
    req->code = -1;
  }
}

int sendNum(int socket, int num, int size, int flags)
{
  int n;
  n = send(socket, (void *)&num, size, flags);
  if (n < 0)
    perror("Error: ");
  return n;
}

int recvNum(int socket, int num, int size, int flags)
{
  int n;
  n = recv(socket, (void *)&num, size, flags);
  if (n < 0)
    perror("Error: ");
  return n;
}
