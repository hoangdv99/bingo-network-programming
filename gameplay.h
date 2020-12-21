#include "list.h"
#include "protocol.h"
#include <sys/types.h>
#include <sys/socket.h>
//For inet_addr()
#include <netinet/in.h>
#include <arpa/inet.h>
#include "handle.h"

void initGame(ROOM* room);
int pickNumber(ROOM *room, int number);
int checkBingo(ROOM* room, USER *user);
void roomThreadFunc(void *arg);