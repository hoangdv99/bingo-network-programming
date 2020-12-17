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

int clientGUI(int serverfd);

int main(int argc, char const *argv[])
{
    // catch wrong input
	if(argc!=3){
		printf("Please input IP address and port number\n");
		return 0;
	}
    int servPort;
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
    clientGUI(sockfd);
    close(sockfd);
    return 0;
}

