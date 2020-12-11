#include <sys/types.h>
#include <sys/socket.h>
//For inet_addr()
#include <netinet/in.h>
#include <arpa/inet.h>
// For ..
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "protocol.h"
#include "helper.h"
#include "client_handle.h"
#define BUFF_SIZE 255

int main(int argc, char const *argv[])
{
    // catch wrong input
	if(argc!=3){
		printf("Please input IP address and port number\n");
		return 0;
	}
    int servPort;
    Request *req = (Request *)malloc(sizeof(Request));
    Response *res = (Response*)malloc(sizeof(Response));
    struct sockaddr_in servaddr;
    //Tạo TCP socket
    int sockfd = socket(PF_INET, SOCK_STREAM, 0);

    if (sockfd == -1) // Neu tao socket bi loi
    {
        perror("CREATE_SOCKET");
        exit(0); // Thoat
    }
    //Step 2: Specify server address
    servPort = atoi(argv[2]);
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(servPort);
    servaddr.sin_addr.s_addr = inet_addr(argv[1]);

    // Them mot tham so ve kich thuoc dia chi
    socklen_t len = sizeof(struct sockaddr_in);

    // Gio ta ket noi den server
    int check = connect(sockfd, (struct sockaddr *)&servaddr, len);
    if (check == -1) // Ket noi bi loi
    {
        perror("CONNECT");
        exit(0);
    }
    // Gio ta se giao tiep voi server qua clientSocket
    // Gui mot message den servser
    //char message[100];
    char sendbuff[BUFF_SIZE];
    while (1)
    {
        memset(sendbuff, '\0', BUFF_SIZE); //initialize buffer
        memset(res->data, '\0', BUFF_SIZE); //clear buff in res->data
        memset(res->message, '\0', BUFF_SIZE); //clear buff in res->message
        inputRequest(sendbuff);
        setOpcodeRequest(req, sendbuff);
        printf("\n%d-%s-%s\n", req->code, req->message, sendbuff);
        switch (req->code)
        {
        case REGISTER:
            registerAccount(sockfd, req, res);
            break;
        case LOGIN:
            login(sockfd, req, res);
            break;
        case DETAIL:
            seeDetail(sockfd, req, res);
            break;
        case LOGOUT:
            logOut(sockfd, req, res);
            break;
        case CREATE_ROOM:
            createRoom(sockfd, req, res);
            break;
        case QUICKJOIN:
            quickJoin(sockfd, req, res);
            break;
        default:
            break;
        }
    }
    
    return 0;
}

