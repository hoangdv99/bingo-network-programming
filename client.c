// #include <sys/types.h>
// #include <sys/socket.h>
// //For inet_addr()
// #include <netinet/in.h>
// #include <arpa/inet.h>
// #include <unistd.h> // for close
// // For ..
// #include <string.h>
// #include <stdlib.h>
// #include <stdio.h>
// #include "protocol.h"
// #include "helper.h"
// #include "client_handle.h"
// #define BUFF_SIZE 255

// int clientGUI(int serverfd);

// int main(int argc, char const *argv[])
// {
//     // catch wrong input
// 	if(argc!=3){
// 		printf("Please input IP address and port number\n");
// 		return 0;
// 	}
//     int servPort;
//     struct sockaddr_in servaddr;
//     //Tạo TCP socket
//     int sockfd = socket(PF_INET, SOCK_STREAM, 0);

//     if (sockfd == -1) // Neu tao socket bi loi
//     {
//         perror("CREATE_SOCKET");
//         exit(0); // Thoat
//     }
//     //Step 2: Specify server address
//     servPort = atoi(argv[2]);
//     servaddr.sin_family = AF_INET;
//     servaddr.sin_port = htons(servPort);
//     servaddr.sin_addr.s_addr = inet_addr(argv[1]);

//     // Them mot tham so ve kich thuoc dia chi
//     socklen_t len = sizeof(struct sockaddr_in);

//     // Gio ta ket noi den server
//     int check = connect(sockfd, (struct sockaddr *)&servaddr, len);
//     if (check == -1) // Ket noi bi loi
//     {
//         perror("CONNECT");
//         exit(0);
//     }
//     clientGUI(sockfd);
//     close(sockfd);
//     return 0;
// }

//hoang test
#include <sys/types.h>
#include <sys/socket.h>
//For inet_addr()
#include <netinet/in.h>
#include <arpa/inet.h>
// For ..
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include "protocol.h"
#include "helper.h"
#include "client_handle.h"
#define BUFF_SIZE 255

//int clientGUI();

void *client_handler(void *arg){
	int clientfd;
	int sendBytes, rcvBytes;
	Response *res;
	pthread_detach(pthread_self());
	clientfd = *(int*) arg;

	while(1){
		rcvBytes = recvRes(clientfd, res, sizeof(Response), 0);
		if (rcvBytes < 0){
			perror("\nError: ");
			break;
		}
		else if (rcvBytes == 0){
			close(clientfd);
			printf("[-]Disconnected from server.\n");
			exit(1);
		}
		printf("SERVER: %s\n", res->message);
		
	}
	close(clientfd);
}


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
    //clientGUI();
    pthread_t tid;
	pthread_create(&tid, NULL, &client_handler,(void *) &sockfd);

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
        case JOIN:
            joinClie(sockfd, req, res);
            break;
        case QUICKJOIN:
            quickJoinClie(sockfd, req, res);
            break;
        case INVITE:
            inviteClie(sockfd, req, res);
            break;
        case KICK:
            kickClie(sockfd, req, res);
            break;
        case OUT_ROOM:
            outRoomClie(sockfd, req, res);
            break;
        case ACCEPT_INVITE:
            acceptInviteClie(sockfd, req, res);
            break;
        case DECLINE_INVITE:
            declineInviteClie(sockfd, req, res);
            break;
        case CLOSE:
            closeGameClie(sockfd, req, res);
            break;
        case READY1:
            readyClie(sockfd, req, res);
            break;
        case UNREADY:
            unreadyClie(sockfd, req, res);
            break;
        case CHECK_READY:
            checkReadyClie(sockfd, req, res);
            break;
        case PLAY:
            playClie(sockfd, req, res);
            break;
        case BINGO:
            bingoClie(sockfd, req, res);
            break;
        case PICK:
            pickClie(sockfd, req, res);
            break;
        case RETURN_ROOM:
            returnRoomClie(sockfd, req, res);
            break;
        case QUIT:
            quitGameClie(sockfd, req, res);
        case TEST:
            testClie(sockfd, req, res);
            break;
        default:
            break;
        }
    }
    
    return 0;
}