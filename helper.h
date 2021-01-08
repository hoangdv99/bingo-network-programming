#ifndef __TOOL__
#define __TOOL__

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "list.h"
#include "protocol.h"
//split message
void splitMessage(char *input, char *code, char *data);
void splitMessageData(char *input, char *data1, char *data2);
int findInArray(int num, USER *arr[], int len);
#endif