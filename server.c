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
#include "handle.h"
#define MAX_LENGTH 1024
#define MAX_CLIENT 20

USER *userListHead = NULL;
ROOM *roomListHead = NULL;
ACCOUNT *accountListHead = NULL;

int main(int argc, char *argv[])
{
    readAccountFile();
    printListAccount();
    char *port_char = argv[1];
    int port_number = atoi(port_char);
    struct sockaddr_in servaddr, clieaddr;
    // Tao server socket
    int sockfd = socket(PF_INET, SOCK_STREAM, 0);
    if (sockfd == -1)
    {
        perror("CREATE SOCKET");
        exit(0);
    }
    // Set reuse option
    int i = 1;
    int check = setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &i, sizeof(int));
    if (check == -1)
    {
        perror("Set reuse");
    }
    // Binding to a address
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

    if (check == -1)
    {
        perror("BIND");
        exit(0);
    }
    //Gio ta cho server lang nghe ket noi
    check = listen(sockfd, 10);
    if (check == -1)
    {
        perror("Listen");
        exit(1);
    }
    // Mỗi client kết nối đến server, nếu được chấp nhận thì server sẽ tạo ra một socket để giao tiếp với client đó đến khi 1 trong 2 bên đóng.
    // Ta đã biết đến forking server ( mỗi khi chấp nhận một connect thì tạo 1 process để xử lý nó ), hoặc interactive server ( liên tục kiểm
    // tra xem có kết nối không.
    // Ở đây ta dùng select(), nó sẽ nhìn (watch) trên nhiều file desciptor, socket cũng là 1 loại file descriptor) để xem có sự kiện nào xảy ra
    // không?
    // Có 3 loại sự kiện trên 1 file desciptor mà ta có xem xét là : ready to read, ready to write, exception.
    // Ở đây ta chỉ quan tâm đến ready to read, sự kiện có 1 kết nối đến server coi như là ột /sự kiện ready to read trên server Socket
    // ta cần có 1 tập chứa các socket
    socklen_t len = sizeof(struct sockaddr_in);
    fd_set readfds;
    fd_set masterfds; // tập readfds để check các socket, 1 tập để lưu lại nhưng thay đổi của tập readfds.
    FD_ZERO(&masterfds);
    FD_ZERO(&readfds);
    FD_SET(sockfd, &masterfds); // add serverSock vào tập masterfds.
    int max_fd = sockfd;
    // struct timeval timeout;
    // timeout.tv_sec = 90; // Server sẽ lắng nghe trong 90s, nếu tham số timeout = NULL thì select sẽ chạy mãi.
    // timeout.tv_usec = 0;
    int n_select;
    do
    {
        memcpy(&readfds, &masterfds, sizeof(masterfds)); // Copy masterfds vao readfds để đợi sự kiện
        n_select = select(max_fd + 1, &readfds, NULL, NULL, NULL);
        // Hàm này sẽ block chương trình đến khi có 1 sự kiên ready to read xảy ra
        if (n_select < 0)
        {
            perror("SELECT");
            exit(0);
        }
        else if (n_select == 0)
        {
            printf("Time out\n");
        }
        else
        {
            for (i = 0; i <= max_fd; i++) // Duyệt tất cả các socket đến max_fd.
            {
                if (FD_ISSET(i, &readfds)) // Nếu serverSock có sự kiện ready to read nghĩa là nó có kết nối mới.
                {
                    int close_fd = 0; // Kiểm tra xem socket có nên được đóng sau khi xử lý không
                    if (i == sockfd)
                    {
                        int newCon = accept(sockfd, (struct sockaddr *)&clieaddr, &len); // Chấp nhận kết nối đó
                        printf("New connection \n");

                        FD_SET(newCon, &masterfds); // Thêm vào masterfds set để check sự kiện.
                        if (newCon > max_fd)
                            max_fd = newCon;
                    }
                    else
                    {
                        Request *req = (Request *)malloc(sizeof(Request));
                        Response *res = (Response *)malloc(sizeof(Response));
                        printf("Receive data in socket %d\n", i);
                        int nrecv = recv(i, req, sizeof(Request), 0);
                        if (nrecv == -1)
                        {
                            printf("In socket %d\n", i);
                            perror("RECEIVE");
                            close_fd = 1;
                        }
                        else if (nrecv != 0)
                        {
                            switch (req->code)
                            {
                            case REGISTER:
                                signUp(i, req, res);
                                break;
                            case LOGIN:
                                logIn(i, req, res);
                                break;
                            case DETAIL:
                                sendDetail(i, req, res);
                                break;
                            case LOGOUT:
                                logOut(i, req, res);
                                break;
                            case CREATE_ROOM:
                                createRoom(i, req, res);
                                break;
                            case INVITE:
                                sendInvite(i, req, res);
                                break;
                            case KICK:
                                kick(i, req, res);
                                break;
                            case QUICKJOIN:
                                quickjoin(i, req, res);
                                break;
                            case JOIN:
                                join(i, req, res);
                                break;
                            case OUT_ROOM:
                                outRoom(i, req, res);
                                break;
                            case ACCEPT_INVITE_REQUEST:
                                acceptInvite(i, req, res);
                                break;
                            case DECLINE_INVITE_REQUEST:
                                declineInvite(i, req, res);
                                break;
                            case EXIT_GAME:
                                exitGame(i, req, res);
                                break;
                            default:
                               
                                break;
                            }
                        }
                        else
                        {
                            printf("@@@");
                            close_fd = 1;
                        }
                    }
                    if (close_fd == 1)
                    {
                        printf("@");
                        FD_CLR(i, &masterfds);
                        close(i);
                    }
                }
            }
        }
    } while (1);
    return 0;
}
