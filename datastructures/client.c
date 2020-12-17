#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>


void *client_handler(void *arg){
	int clientfd;
	int sendBytes, rcvBytes;
	char buff[255];
	pthread_detach(pthread_self());
	clientfd = *(int*) arg;

	while(1){
		rcvBytes = recv(clientfd, buff, 255, 0);
		if (rcvBytes < 0){
			perror("\nError: ");
			break;
		}
		else if (rcvBytes == 0){
			close(clientfd);
			printf("[-]Disconnected from server.\n");
			exit(1);
		}
		buff[rcvBytes] = '\0';
		printf("SERVER: %s\n", buff);
		
	}
	close(clientfd);
}




int main(int argc, char *argv[]){
	if(argc!=3){
		printf("Please input IP address and port number\n");
		return 0;
	}

	char *ip_address = argv[1];
	char *port_number = argv[2];
	int port = atoi(port_number);


	int clientSocket, ret;
	struct sockaddr_in serverAddr;
	char buffer[1024];

	clientSocket = socket(AF_INET, SOCK_STREAM, 0);
	if(clientSocket < 0){
		printf("[-]Error in connection.\n");
		exit(1);
	}
	printf("[+]Client Socket is created.\n");

	memset(&serverAddr, '\0', sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(port);
	serverAddr.sin_addr.s_addr = inet_addr(ip_address);

	ret = connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
	if(ret < 0){
		printf("[-]Error in connection.\n");
		exit(1);
	}
	printf("[+]Connected to Server.\n");

	int login;
	int state = 0;
	pthread_t tid;
	pthread_create(&tid, NULL, &client_handler,(void *) &clientSocket);


	while(1){
		
		
		gets(buffer);
		
		buffer[strlen(buffer)]=='\0';
		
		send(clientSocket, buffer, strlen(buffer), 0);

		if((strcmp(buffer, ":exit") == 0) || (strlen(buffer)== 0)){
			close(clientSocket);
			printf("[-]Disconnected from server.\n");
			exit(1);
		}
		
	}

	return 0;
}