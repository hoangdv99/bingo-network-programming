//Example code: A simple server side code, which echos back the received message.
//Handle multiple socket connections with select and fd_set on Linux
#include <stdio.h>
#include <string.h> //strlen
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>	   //close
#include <arpa/inet.h> //close
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/time.h> //FD_SET, FD_ISSET, FD_ZERO macros
#include <stdbool.h>
#include <time.h>
#include "list.h"

#define TRUE 1
#define FALSE 0

// Initialize ------------------------------------------------------------------------------------------

char *itoa(int value, char *result, int base)
{
	// check that the base if valid
	if (base < 2 || base > 36)
	{
		*result = '\0';
		return result;
	}

	char *ptr = result, *ptr1 = result, tmp_char;
	int tmp_value;

	do
	{
		tmp_value = value;
		value /= base;
		*ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz"[35 + (tmp_value - value * base)];
	} while (value);

	// Apply negative sign
	if (tmp_value < 0)
		*ptr++ = '-';
	*ptr-- = '\0';
	while (ptr1 < ptr)
	{
		tmp_char = *ptr;
		*ptr-- = *ptr1;
		*ptr1++ = tmp_char;
	}
	return result;
}
// Linked list -------------------------------

// End FILE ----------------------------------

int main(int argc, char *argv[])
{
	if (argc == 1)
	{
		printf("Please input port number\n");
		return 0;
	}
	char *port_number = argv[1];
	int port = atoi(port_number);

	int opt = TRUE;
	int master_socket, addrlen, new_socket, client_socket[30],
		max_clients = 30, activity, i, valread, sd;
	int max_sd;
	struct sockaddr_in address;

	char buffer[1025]; //data buffer of 1K

	//set of socket descriptors
	fd_set readfds;

	//a message
	char *message = "ECHO Daemon v1.0 \r\n";

	//initialise all client_socket[] to 0 so not checked
	for (i = 0; i < max_clients; i++)
	{
		client_socket[i] = 0;
	}

	//create a master socket
	if ((master_socket = socket(AF_INET, SOCK_STREAM, 0)) == 0)
	{
		perror("socket failed");
		exit(EXIT_FAILURE);
	}

	//set master socket to allow multiple connections ,
	//this is just a good habit, it will work without this
	if (setsockopt(master_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&opt,
				   sizeof(opt)) < 0)
	{
		perror("setsockopt");
		exit(EXIT_FAILURE);
	}

	//type of socket created
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(port);

	//bind the socket to localhost port 8888
	if (bind(master_socket, (struct sockaddr *)&address, sizeof(address)) < 0)
	{
		perror("bind failed");
		exit(EXIT_FAILURE);
	}
	printf("Listener on port %d \n", port);

	//try to specify maximum of 3 pending connections for the master socket
	if (listen(master_socket, 3) < 0)
	{
		perror("listen");
		exit(EXIT_FAILURE);
	}

	//accept the incoming connection
	addrlen = sizeof(address);
	puts("Waiting for connections ...");
	int count = 0;

	//Read file complete----------- Initialize complete

	//init value -------------------------------
	bool LogedIn[max_clients];
	char LoginAcc[max_clients][50];
	//char choice[10];
	//int ch;
	int c[max_clients];
	for (i = 0; i < max_clients; i++)
	{
		LogedIn[i] = false;
		c[i] = 0;
	}

	//------------------------------------------
	int online = 0;
	while (online != 2)
	{

		//clear the socket set
		FD_ZERO(&readfds);

		//add master socket to set
		FD_SET(master_socket, &readfds);
		max_sd = master_socket;

		//add child sockets to set
		for (i = 0; i < max_clients; i++)
		{
			//socket descriptor
			sd = client_socket[i];

			//if valid socket descriptor then add to read list
			if (sd > 0)
				FD_SET(sd, &readfds);

			//highest file descriptor number, need it for the select function
			if (sd > max_sd)
				max_sd = sd;
		}

		//wait for an activity on one of the sockets , timeout is NULL ,
		//so wait indefinitely
		activity = select(max_sd + 1, &readfds, NULL, NULL, NULL);

		if ((activity < 0) && (errno != EINTR))
		{
			printf("select error");
		}

		//If something happened on the master socket ,
		//then its an incoming connection
		if (FD_ISSET(master_socket, &readfds))
		{
			if ((new_socket = accept(master_socket,
									 (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0)
			{
				perror("accept");
				exit(EXIT_FAILURE);
			}

			//inform user of socket number - used in send and receive commands
			printf("New connection , socket fd is %d , ip is : %s , port : %d \n", new_socket, inet_ntoa(address.sin_addr), ntohs(address.sin_port));

			//add new socket to array of sockets
			for (i = 0; i < max_clients; i++)
			{

				//if position is empty
				if (client_socket[i] == 0)
				{
					client_socket[i] = new_socket;
					printf("Adding to list of sockets as %d\n", i);
					char name[30];
					strcpy(name, "Bravo_");
					char snum[5];
					itoa(new_socket, snum, 10);
					strcat(name, snum);
					printf("Connect from %s \n", name);
					USER *acc = makenode(name, new_socket);
					printf("%s - %d \n", acc->username, acc->clientfd);
					if (findRoom(1) == NULL)
					{
						printf("create room 1\n");
						insertRoom(1, acc);
					}
					else
					{
						printf("Insert to room 1\n");
						insertPlayer(1, acc);
					}
					online++;
					break;
				}
			}
		}
	}
	printf("LET'S PLAYYYYYYYYY!!!!!!!!!!!------------------\n");

	int p = 0;
	startGame(1);
	ROOM *room = findRoom(1);
	;
	int playing;
	struct timeval timeout;
	time_t now;
	time_t end_time;
	int remain_time = 15;
	int bingo = 0;
	char winner[50];

	while (bingo != 1)
	{
		timeout.tv_sec = remain_time;
		timeout.tv_usec = 0;
		now = time(NULL);
		end_time = now + remain_time;

		if (room != NULL)
		{
			playing = room->player[p]->clientfd;
		}
		//clear the socket set
		FD_ZERO(&readfds);

		//add master socket to set
		FD_SET(master_socket, &readfds);
		max_sd = master_socket;

		//add child sockets to set
		for (i = 0; i < max_clients; i++)
		{
			//socket descriptor
			sd = client_socket[i];

			//if valid socket descriptor then add to read list
			if (sd > 0)
				FD_SET(sd, &readfds);

			//highest file descriptor number, need it for the select function
			if (sd > max_sd)
				max_sd = sd;
		}

		for (int i = 0; i < room->playerAmount; i++)
		{
			sd = room->player[i]->clientfd;
			if (i == p)
			{
				send(sd, "Your turn", strlen("Your turn"), 0);
			}
			else
			{
				char resmessage[50];
				strcpy(resmessage, "Player ");
				strcat(resmessage, room->player[p]->username);
				strcat(resmessage, " turn");
				send(sd, resmessage, strlen(resmessage), 0);
			}
		}

		//wait for an activity on one of the sockets
		//so wait indefinitely

		activity = select(max_sd + 1, &readfds, NULL, NULL, &timeout);

		if ((activity < 0) && (errno != EINTR))
		{
			printf("select error");
		}

		if (activity == 0)
		{
			if (p == room->playerAmount - 1)
			{
				p = 0;
			}
			else
			{
				p++;
			}
			printf(" timed out.  Bravo[%d] turn\n", room->player[p]->clientfd);
			remain_time = 15;
			continue;
		}

		//else its some IO operation on some other socket
		for (i = 0; i < max_clients; i++)
		{
			//printf("%d\n",i);

			sd = client_socket[i];
			int j = 0;
			if (FD_ISSET(sd, &readfds))
			{
				valread = read(sd, buffer, 1024);
				buffer[valread] = '\0';
				if ((valread == 0) || (strcmp(buffer, ":exit") == 0))
				{
					//Somebody disconnected , get his details and print
					getpeername(sd, (struct sockaddr *)&address, (socklen_t *)&addrlen);
					printf("Host disconnected , ip %s , port %d \n",
						   inet_ntoa(address.sin_addr), ntohs(address.sin_port));

					close(sd);
					client_socket[i] = 0;
				}
				else if (strcmp(buffer, "TEST") == 0)
				{
					int check;
					for (int i = 0; i < room->playerAmount; i++) {
						if(room->player[i]->clientfd == sd) {
							check = i;
						}
					}
					BINGOTEST(1, room->player[check]->username, 3, 1);
					send(sd, "Now say BINGO", strlen("Now say BINGO"), 0);
					if ((end_time - time(NULL)) != 0)
					{
						remain_time = end_time - time(NULL);
						printf("Bravo[%d] turn %d s remain\n", playing, remain_time);
					}
				}
				else if (strcmp(buffer, "BINGO") == 0)
				{
					int check;
					for (int i = 0; i < room->playerAmount; i++) {
						if(room->player[i]->clientfd == sd) {
							check = i;
						}
					}
					int res = checkBingo(1, room->player[check]->username);
					if ((res == 1) && (bingo == 0))
					{
						send(sd, "Bingo yoo", strlen("Bingo yoo"), 0);
						bingo = 1;
						strcpy(winner, room->player[check]->username);
					}
					else
					{
						send(sd, "Xaoloz", strlen("Xaoloz"), 0);
						if ((end_time - time(NULL)) != 0)
						{
							remain_time = end_time - time(NULL);
							printf("Bravo[%d] turn %d s remain\n", playing, remain_time);
						}
					}
				}

				else
				{

					if (sd == playing)
					{
						printf("Playerfd[%d]: - %s\n", sd, buffer);

						char message[50];
						int pick = atoi(buffer);
						if (pick == 0 || pick > 25)
						{
							strcpy(message, "Wrong request!");
							send(sd, message, strlen(message), 0);
						}
						else
						{
							int res = pickNumber(1, pick);
							if (res == 0)
							{
								strcpy(message, "Already picked!");
								send(sd, message, strlen(message), 0);
							}
							else
							{
								strcpy(message, "You just pick ");
								strcat(message, buffer);
								remain_time = 15;

								for (int i = 0; i < room->playerAmount; i++)
								{
									sd = room->player[i]->clientfd;
									if (i == p)
									{
										send(sd, message, strlen(message), 0);
									}
									else
									{
										char resmessage[50];
										strcpy(resmessage, "Player ");
										strcat(resmessage, room->player[p]->username);
										strcat(resmessage, " pick");
										strcat(resmessage, buffer);
										send(sd, resmessage, strlen(resmessage), 0);
									}
								}

								if (p == room->playerAmount - 1)
								{
									p = 0;
								}
								else
								{
									p++;
								}
							}
						}
						
					}
					else
					{
						send(sd, "Not your turn", strlen("Not your turn"), 0);
						if ((end_time - time(NULL)) != 0)
						{
							remain_time = end_time - time(NULL);
							printf("Bravo[%d] turn %d s remain\n", playing, remain_time);
						}
						else
						{
							if (p == room->playerAmount - 1)
							{
								p = 0;
							}
							else
							{
								p++;
							}
							remain_time = 15;
						}
					}
				}
			}
		}
	}
	printf("WINNER IS %s\n", winner);
	for (int i = 0; i < room->playerAmount; i++)
	{
		sd = room->player[i]->clientfd;
		if (strcmp(winner, room->player[i]->username) == 0)
		{
			send(sd, "You win", strlen("Your win"), 0);
		}
		else
		{
			char resmessage[50];
			strcpy(resmessage, "Player ");
			strcat(resmessage, room->player[p]->username);
			strcat(resmessage, " has won");
			send(sd, resmessage, strlen(resmessage), 0);
		}
		getpeername(sd, (struct sockaddr *)&address, (socklen_t *)&addrlen);
		printf("Host disconnected , ip %s , port %d \n",
			   inet_ntoa(address.sin_addr), ntohs(address.sin_port));

		close(sd);
		client_socket[i] = 0;
	}
	//playGame(1, readfds, master_socket, max_sd);

	return 0;
}
