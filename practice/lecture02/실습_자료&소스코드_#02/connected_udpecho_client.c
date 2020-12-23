#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 1024

void error_handling(char *message);
int create_socket(char *ip, int port);
void exchange_data(int sock);

int main(int argc, char *argv[])
{
    int sock;
    int i;

    if (argc != 3)
    {
        printf("Usage : %s <IP> <port>\n", argv[0]);
        exit(1);
    }

    sock = create_socket(argv[1], atoi(argv[2]));
    exchange_data(sock);

    return 0;
}

int create_socket(char *ip, int port)
{
    int sock;
    char message[BUF_SIZE];
    int str_len;
    socklen_t adr_sz;

    struct sockaddr_in serv_adr, from_adr;

    sock = socket(PF_INET, SOCK_DGRAM, 0);
    if (sock == -1)
        error_handling("socket() error");

    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = inet_addr(ip);
    serv_adr.sin_port = htons(port);

    connect(sock, (struct sockaddr *)&serv_adr, sizeof(serv_adr));

    return sock;
}

void exchange_data(int sock)
{
    char message[BUF_SIZE] = {
        0,
    };
    int str_len;

    write(sock, "Hello", strlen("Hello"));
    puts("Send : Hello");

    write(sock, "World", strlen("World"));
    puts("Send : World");

    str_len = read(sock, message, BUF_SIZE - 1);
    message[str_len] = 0;
    printf("Message from server: %s ", message);

    str_len = read(sock, message, BUF_SIZE - 1);
    message[str_len] = 0;
    printf("%s\n", message);

    close(sock);
}

void error_handling(char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}
