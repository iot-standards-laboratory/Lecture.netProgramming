#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <sys/select.h>
#include <sys/wait.h>
#include <sys/uio.h>

#define BUF_SIZE 100
void error_handling(char *buf);
int create_serversock(int port);
int exchange_data(int clnt_sock);
int create_clientsock(int serv_sock);
void sigchld_init();
void read_childproc(int sig);

int main(int argc, char *argv[])
{
    int serv_sock, clnt_sock;
    struct timeval timeout;
    fd_set reads, cpy_reads;
    int fd_max, str_len, fd_num, i;
    if (argc != 2)
    {
        printf("Usage : %s <port>\n", argv[0]);
        exit(1);
    }

    sigchld_init();
    serv_sock = create_serversock(atoi(argv[1]));

    FD_ZERO(&reads);
    FD_SET(serv_sock, &reads);
    fd_max = serv_sock;

    while (1)
    {
        cpy_reads = reads;
        timeout.tv_sec = 5;
        timeout.tv_usec = 5000;

        if ((fd_num = select(_________, _________, _________, _________, _________)) == -1 || fd_num == 0) // 수정해보세요
            continue;

        for (i = 0; i < _________; i++) // 수정해보세요
        {
            if (FD_ISSET(i, _________)) // 수정해보세요
            {
                if (i == serv_sock) // connection request!
                {
                    clnt_sock = create_clientsock(serv_sock);
                    FD_SET(_________, _________); // 수정해보세요
                    if (fd_max < clnt_sock)
                        fd_max = clnt_sock;
                    printf("connected client: %d \n", clnt_sock);
                }
                else // read message!
                {
                    int retValue = exchange_data(i);
                    if (retValue)
                    {
                        if (retValue == 1)
                            printf("disconnected client: %d \n", i);

                        else if (retValue == 2)
                            printf("Created Child Process\n");
                        FD_CLR(_________, &reads); // 수정해보세요
                    }
                }
            }
        }
    }

    close(serv_sock);

    return 0;
}

void sigchld_init()
{
    struct sigaction act;
    act.sa_handler = read_childproc;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    sigaction(SIGCHLD, &act, 0);
}

void read_childproc(int sig)
{
    int status;
    pid_t id = waitpid(-1, &status, WNOHANG);

    if (WIFEXITED(status))
    {
        printf("Removed proc id: %d \n", id);
        printf("Child send: %d \n", WEXITSTATUS(status));
    }
}

int create_serversock(int port)
{
    int serv_sock, clnt_sock;
    int str_len, i;

    struct sockaddr_in serv_adr;

    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    if (serv_sock == -1)
        error_handling("socket() error");

    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_adr.sin_port = htons(port);

    if (bind(serv_sock, (struct sockaddr *)&serv_adr, sizeof(serv_adr)) == -1)
        error_handling("bind() error");

    if (listen(serv_sock, 3) == -1)
        error_handling("listen() error");

    return serv_sock;
}

int create_clientsock(int serv_sock)
{
    struct sockaddr_in clnt_adr;
    socklen_t clnt_adr_sz;
    int clnt_sock;

    clnt_adr_sz = sizeof(clnt_adr);
    clnt_sock = accept(serv_sock, (struct sockaddr *)&clnt_adr, &clnt_adr_sz);

    return clnt_sock;
}

int exchange_data(int clnt_sock)
{
    int str_len, delay;
    char message[BUF_SIZE];
    struct iovec ivec[2], ovec[2];
    struct sockaddr_in clnt_adr;
    socklen_t clnt_adr_sz;
    pid_t pid;

    ivec[0].iov_base = &delay;
    ivec[0].iov_len = sizeof(delay);
    ivec[1].iov_base = &str_len;
    ivec[1].iov_len = sizeof(str_len);

    ovec[0].iov_base = &str_len;
    ovec[0].iov_len = sizeof(str_len);
    ovec[1].iov_base = message;

    if (!readv(clnt_sock, ivec, 2))
    {
        close(clnt_sock);
        return 1;
    }
    else if (delay >= 3)
    {
        pid = fork();
        if (pid == 0)
        {
            while (1)
            {
                sleep(delay);
                read(clnt_sock, message, str_len);
                message[str_len] = 0;

                ovec[1].iov_len = str_len;
                writev(clnt_sock, ovec, 2);
                getpeername(clnt_sock, (struct sockaddr *)&clnt_adr, &clnt_adr_sz);
                printf("receive from %s on child: %s", inet_ntoa(clnt_adr.sin_addr), message);

                if (!readv(clnt_sock, ivec, 2))
                {
                    close(clnt_sock);
                    exit(0);
                }
            }
        }
        else
        {
            close(clnt_sock);
            return 2;
        }
    }

    sleep(delay);
    str_len = read(clnt_sock, message, str_len);
    message[str_len] = 0;

    ovec[1].iov_len = str_len;
    writev(clnt_sock, ovec, 2);
    getpeername(clnt_sock, (struct sockaddr *)&clnt_adr, &clnt_adr_sz);
    printf("receive from %s : %s", inet_ntoa(clnt_adr.sin_addr), message);

    return 0;
}

void error_handling(char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}