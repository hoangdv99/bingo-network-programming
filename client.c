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
#define BUFF_SIZE 255

int main(int argc, char const *argv[])
{
    int servPort;
    Request *req = (Request *)malloc(sizeof(Request));
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
    req->code = LOGIN;
    strcpy(req->message, "hoang@123");
    int n_sent = send(sockfd, req, sizeof(Request), 0);
    if (n_sent == -1) // Gui loi
    {
        perror("SEND");
        exit(0);
    }
    printf("Sent %d bytes to server\n", n_sent);
    printf("Waiting for reply\n");
    //Doi server   gui lai
    Response *res = (Response *)malloc(sizeof(Response));
    int n_recv = recv(sockfd, res, sizeof(Response), 0);
    if (n_recv == -1)
    {
        perror("RECEIVE");
        exit(0);
    }
    if (n_recv == 0)
    {
    }
    printf("Received string with leng : %d\n", res->code);
    printf("Received string with leng : %s\n", res->message);
    while (1)
    {
        /* code */
    }
    
    return 0;
}
