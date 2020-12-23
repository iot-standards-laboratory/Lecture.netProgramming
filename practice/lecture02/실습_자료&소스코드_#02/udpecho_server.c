#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 1024

void error_handling(char *message);
int create_socket(int port);
void exchange_data(int sock);

int main(int argc, char *argv[])
{
    int sock;
    int i;

    if (argc != 2)
    {
        printf("Usage : %s <port>\n", argv[0]);
        exit(1);
    }

    sock = create_socket(atoi(argv[1]));

    for (i = 0; i < 5; i++)
        exchange_data(sock);

    close(sock);
    return 0;
}

int create_socket(int port)
{
    int sock;
    struct sockaddr_in my_adr;

    sock = socket(PF_INET, SOCK_DGRAM, 0);
    if (sock == -1)
        error_handling("socket() error");

    memset(&my_adr, 0, sizeof(my_adr));
    my_adr.sin_family = AF_INET;
    my_adr.sin_addr.s_addr = htonl(INADDR_ANY);
    my_adr.sin_port = htons(port);

    if (bind(sock, (struct sockaddr *)&my_adr, sizeof(my_adr)) == -1)
        error_handling("bind() error");

    return sock;
}

void exchange_data(int sock)
{
    int str_len;
    char message[BUF_SIZE];
    socklen_t clnt_adr_sz;
    struct sockaddr_in clnt_adr;

    getchar();

    clnt_adr_sz = sizeof(clnt_adr);
    str_len = recvfrom(sock, message, BUF_SIZE, 0, (struct sockaddr *)&clnt_adr, &clnt_adr_sz);
    message[str_len] = 0;
    printf("receive from %s : %s\n", inet_ntoa(clnt_adr.sin_addr), message);
    sendto(sock, message, str_len, 0, (struct sockaddr *)&clnt_adr, clnt_adr_sz);

    clnt_adr_sz = sizeof(clnt_adr);
    str_len = recvfrom(sock, message, BUF_SIZE, 0, (struct sockaddr *)&clnt_adr, &clnt_adr_sz);
    message[str_len] = 0;
    printf("receive from %s : %s\n", inet_ntoa(clnt_adr.sin_addr), message);
    sendto(sock, message, str_len, 0, (struct sockaddr *)&clnt_adr, clnt_adr_sz);
}

void error_handling(char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}
