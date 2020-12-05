#ifndef __HANDLE__
#define __HANDLE__
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#define MAX_LENGTH 255
#define MAX_CLIENT 20
#define MAX_LENGTH 255
#include "protocol.h"
void login(int socket, Request *req, Response *res);
 
#endif