#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/select.h>
#include <sys/uio.h>
#define BUF_SIZE 100
void error_handling(char *buf);
int create_serversock(int port);
int exchange_data(int clnt_sock);
int create_clientsock(int serv_sock);

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

    serv_sock = create_serversock(atoi(argv[1]));

    FD_ZERO(&reads);
    FD_SET(serv_sock, &reads);
    fd_max = serv_sock;

    while (1)
    {
        cpy_reads = reads;
        timeout.tv_sec = 5;
        timeout.tv_usec = 5000;

        if ((fd_num = select(fd_max + 1, &cpy_reads, 0, 0, &timeout)) == -1)
            break;
        if (fd_num == 0)
            continue;

        for (i = 0; i < fd_max + 1; i++)
        {
            if (FD_ISSET(i, &cpy_reads))
            {
                if (i == serv_sock) // connection request!
                {
                    clnt_sock = create_clientsock(serv_sock);
                    FD_SET(clnt_sock, &reads);
                    if (fd_max < clnt_sock)
                        fd_max = clnt_sock;
                    printf("connected client: %d \n", clnt_sock);
                }
                else // read message!
                {
                    if (!exchange_data(i))
                    {
                        printf("disconnected client: %d \n", i);
                        FD_CLR(i, &reads);
                    }
                }
            }
        }
    }

    close(serv_sock);
    return 0;
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

    if (listen(serv_sock, 5) == -1)
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
    int str_len;
    char buf[BUF_SIZE] = {
        0,
    };
    char msg1[] = "HTML";
    char msg2[] = "CSS";
    char msg3[] = "JS";

    struct iovec vec[3];

    read(clnt_sock, &str_len, sizeof(int));
    str_len = read(clnt_sock, buf, str_len);
    buf[str_len] = 0;

    printf("Request from client: %s ", buf);

    vec[0].iov_base = msg1;
    vec[0].iov_len = strlen(msg1);

    vec[1].iov_base = msg2;
    vec[1].iov_len = strlen(msg2);

    vec[2].iov_base = msg3;
    vec[2].iov_len = strlen(msg3);

    str_len = writev(clnt_sock, vec, 3);
    puts("");
    printf("Write bytes: %d \n", str_len);

    close(clnt_sock);
    return 0;
}

void error_handling(char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}