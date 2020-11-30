#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/wait.h>
#include <errno.h>
#include <arpa/inet.h>
#include "protocol.h"
#define MAX_LENGTH 255
#define MAX_CLIENT 20

int main(int argc, char *argv[])
{

    FILE *f;
    char *port_char = argv[1];
    int port_number = atoi(port_char);
    int sockfd, connfd, len, maxi, client[FD_SETSIZE], nready, i, maxfd;
    struct sockaddr_in servaddr, clieaddr;
    char buff[MAX_LENGTH];
    char sendBuff[MAX_LENGTH];
    char recvBuff[MAX_LENGTH];
    pid_t pid;
    fd_set readfds, allset;
    GAMEPLAY_STATUS status;
    Request *req = (Request *)malloc(sizeof(Request));
    Response *res = (Response *)malloc(sizeof(Response));

     //Step 1: Construct a TCP socket to listen connection request
    sockfd = socket(PF_INET, SOCK_STREAM, 0);
    if (sockfd == -1)
    {
        perror("CREATE_SOCKET");
        exit(0);
    }

    //Step 2: Bind address to socket
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(port_number);

    if ((bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr))) != 0)
    {
        perror("BLIND");
        exit(0);
    }

    //Step 3: Listen request from client
     if (listen(sockfd, MAX_CLIENT) == -1){
        perror("\nError: ");
        return 0;
    }

    maxfd = sockfd;    //init
    maxi = -1;              //index into clients[] array
    for (i = 0; i < FD_SETSIZE; i++){
        client[i] = -1;     //-1 indicates avaiable entry
        status
    }
    
    FD_ZERO(&allset);
    FD_SET(sockfd, &allset);

    //Step 4: communicate with clients
    while (1){
        readfds = allset;
        nready = select(maxfd + 1, &readfds, NULL, NULL, NULL);
        if(nready < 0){
            perror("\nError: ");
            exit(0);
        }

        if(FD_ISSET(sockfd, &readfds)){     //new connection
            len = sizeof(clieaddr);
            if((connfd = accept(sockfd, (struct sockaddr*)&clieaddr, &len)) < 0){
                perror("\nError: ");
            }else{
                printf("New connection from %s\n", inet_ntoa(clieaddr.sin_addr));   //print client ip
                for(i = 0; i < FD_SETSIZE; i++){
                    if(client[i] < 0){
                        client[i] = connfd;
                        break;
                    }
                }

                if(i == FD_SETSIZE){
                    printf("\nToo many clients");
                    close(connfd);
                }
                FD_SET(connfd, &allset); /* add new descriptor to set */
                if (connfd > maxfd)
                    maxfd = connfd; /* for select */
                if (i > maxi)
                    maxi = i; /* max index in client[] array */

                if (--nready <= 0)
                    continue; /* no more readable descriptors */
            }
        }
    }
    
    return 0;
}