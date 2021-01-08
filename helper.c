#include "helper.h"
#define MAX_LENGTH 255
void splitMessage(char *input, char *code, char *data)
{
	int i, codeLength = 0, dataLength = 0;
	for (i = 0; input[i] != ' '; i++)
	{
		code[codeLength++] = input[i];
	}
	code[codeLength] = '\0';
	i++;
	for (; i < strlen(input); i++)
	{
		data[dataLength++] = input[i];
	}
	data[dataLength] = '\0';
}

void splitMessageData(char *input, char *username, char *password)
{
	int i, usernameLength = 0, passwordLength = 0;
	for (i = 0; input[i] != ' '; i++)
	{
		username[usernameLength++] = input[i];
	}
	username[usernameLength] = '\0';
	i++;
	for (; i < strlen(input); i++)
	{
		password[passwordLength++] = input[i];
	}
	password[passwordLength] = '\0';
}

int findInArray(int num, USER *arr[], int len)
{
	for (int i = 0; i < len; i++)
	{
		if (num == arr[i]->clientfd)
			return 1;
	}
	return 0;
}