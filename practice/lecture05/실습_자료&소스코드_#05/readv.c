#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/uio.h>
#define BUF_SIZE 1024
void error_handling(char *message);
int create_clientsock(char *ip, int port);
void exchange_data(int sock);

int main(int argc, char *argv[])
{
    int sock;

    struct sockaddr_in serv_adr;

    if (argc != 3)
    {
        printf("Usage : %s <IP> <port>\n", argv[0]);
        exit(1);
    }

    sock = create_clientsock(argv[1], atoi(argv[2]));
    exchange_data(sock);
    return 0;
}

void exchange_data(int sock)
{
    int str_len;
    char req[] = "GET";
    char buf1[BUF_SIZE] = {
        0,
    };
    char buf2[BUF_SIZE] = {
        0,
    };
    char buf3[BUF_SIZE] = {
        0,
    };
    struct iovec vec[3];

    str_len = strlen(req);
    write(sock, &str_len, sizeof(int));
    write(sock, req, str_len);
    puts("Send Request GET");

    vec[0].iov_base = buf1;
    vec[0].iov_len = 4;

    vec[1].iov_base = buf2;
    vec[1].iov_len = 3;

    vec[2].iov_base = buf3;
    vec[2].iov_len = 2;

    readv(sock, vec, 3);

    printf("Read bytes: %d \n", str_len);
    printf("First message: %s \n", buf1);
    printf("Second message: %s \n", buf2);
    printf("Third message: %s \n", buf3);

    close(sock);
}

int create_clientsock(char *ip, int port)
{
    int sock;
    struct sockaddr_in serv_adr;

    sock = socket(PF_INET, SOCK_STREAM, 0);
    if (sock == -1)
        error_handling("socket() error");

    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = inet_addr(ip);
    serv_adr.sin_port = htons(port);

    if (connect(sock, (struct sockaddr *)&serv_adr, sizeof(serv_adr)) == -1)
        error_handling("connect() error!");
    else
        puts("Connected...........");

    return sock;
}

void error_handling(char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}