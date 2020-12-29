#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
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
    char message[BUF_SIZE];
    int str_len;

    write(sock, "123", strlen("123"));
    write(sock, "456", strlen("456"));
    write(sock, "78", strlen("78"));

    fputs("Input message(close or oob): ", stdout);
    fgets(message, BUF_SIZE, stdin);

    if (!strcmp(message, "close\n"))
        close(sock);
    else if (!strcmp(message, "oob\n"))
    {
        send(sock, "90", strlen("90"), MSG_OOB);
        str_len = read(sock, message, strlen("0K"));
        message[str_len] = 0;
        puts(message);
        sleep(5);
        puts("ByeBye!!");
        close(sock);
    }
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