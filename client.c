#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <string.h>
#include "protocol.h"
#include "gameplay.h"
#define BUFF_SIZE 255
int main(int argc, char const *argv[])
{
    int sockfd, servPort;
    struct sockaddr_in servaddr;
    GAMEPLAY_STATUS status = ANAUTH;
    Request *req = (Request*)malloc(sizeof(Request));
    Response *res = (Response*)malloc(sizeof(Response));
    char buff[BUFF_SIZE], username[BUFF_SIZE];

    if(argc != 3){
        printf("Wrong params\n");
        exit(0);
    }
    //Step 1: Construct socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    //Step 2: Specify server address
    servPort = atoi(argv[2]);
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(servPort);
    servaddr.sin_addr.s_addr = inet_addr(argv[1]);

    //Step 3: Request to connect server
    if (connect(sockfd, (struct sockaddr *)&servaddr, sizeof(struct sockaddr)) < 0)
    {
        printf("\nError!Can not connect to sever! Client exit imediately! ");
        exit(0);
    }

    //Step 4: communicate with server
    while(1){
        switch (status)
        {
        case ANAUTH:
            printf("Syntax: LOGIN username");
            memset(buff, '\0', (strlen(buff)+1));
            fgets(buff, BUFF_SIZE, stdin);
            buff[strlen(buff) - 1] = '\0';
            setOpcodeRequest(req, buff);
            sendReq(sockfd, req, sizeof(Request), 0);

            recvRes(sockfd, res, sizeof(Response), 0);
            readMessageResponse(res);
            status = res->status;
            if (res->data != NULL)
            {
                memset(username, '\0', (strlen(username) + 1));
                strcpy(username, res->data);
            }
            break;
        
        default:
            break;
        }
    }
    return 0;
}
