#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 1024

void error_handling(char *message);
int create_serversock(int port);
int create_clientsock(int serv_sock);
void exchange_data(int clnt_sock);

int main(int argc, char *argv[])
{

    int serv_sock, clnt_sock;
    int i;

    if (argc != 2)
    {
        printf("Usage : %s <port>\n", argv[0]);
        exit(1);
    }

    serv_sock = create_serversock(atoi(argv[1]));

    for (i = 0; i < 5; i++)
    {
        clnt_sock = create_clientsock(serv_sock);
        if (clnt_sock == -1)
            error_handling("accept() error");
        else
            printf("Connected client %d \n", i + 1);

        exchange_data(clnt_sock);
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

void exchange_data(int clnt_sock)
{
    int str_len;
    char message[BUF_SIZE];
    struct sockaddr_in clnt_adr;
    socklen_t clnt_adr_sz;

    getchar();
    read(clnt_sock, &str_len, sizeof(int));
    str_len = read(clnt_sock, message, str_len);
    message[str_len] = 0;

    str_len = strlen(message);
    write(clnt_sock, &str_len, sizeof(int));
    write(clnt_sock, message, str_len);
    getpeername(clnt_sock, (struct sockaddr *)&clnt_adr, &clnt_adr_sz);
    printf("receive from %s : %s\n", inet_ntoa(clnt_adr.sin_addr), message);

    read(clnt_sock, &str_len, sizeof(int));
    str_len = read(clnt_sock, message, str_len);
    message[str_len] = 0;

    str_len = strlen(message);
    write(clnt_sock, &str_len, sizeof(int));
    write(clnt_sock, message, str_len);
    printf("receive from %s : %s\n", inet_ntoa(clnt_adr.sin_addr), message);

    close(clnt_sock);
}

void error_handling(char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}