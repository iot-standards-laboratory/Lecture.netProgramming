#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#define BUF_SIZE 30
void error_handling(char *message);
int create_serversock(int port);
int create_clientsock(int serv_sock);
void exchange_data();
void urg_handler(int signo);
void urg_init();

int serv_sock, clnt_sock;

int main(int argc, char *argv[])
{
    int i;

    if (argc != 2)
    {
        printf("Usage : %s <port>\n", argv[0]);
        exit(1);
    }

    serv_sock = create_serversock(atoi(argv[1]));

    clnt_sock = create_clientsock(serv_sock);
    if (clnt_sock == -1)
        error_handling("accept() error");
    else
        printf("Connected client %d \n", i + 1);

    urg_init();
    exchange_data();

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

void urg_init()
{
    int state;
    struct sigaction act;
    act.sa_handler = urg_handler;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;

    fcntl(clnt_sock, F_SETOWN, getpid());
    state = sigaction(SIGURG, &act, 0);
}

void urg_handler(int signo)
{
    int str_len;
    char buf[BUF_SIZE];
    struct sockaddr_in clnt_adr;
    socklen_t clnt_adr_sz;

    str_len = recv(clnt_sock, buf, sizeof(buf) - 1, MSG_OOB);
    buf[str_len] = 0;
    printf("Urgent message: %s \n", buf);

    str_len = write(clnt_sock, "OK", strlen("OK"));
}

void exchange_data()
{
    int str_len;
    char buf[BUF_SIZE];
    struct sockaddr_in clnt_adr;
    socklen_t clnt_adr_sz;

    while ((str_len = recv(clnt_sock, buf, sizeof(buf), MSG_WAITALL)) != 0)
    {
        if (str_len == -1)
            continue;
        buf[str_len] = 0;
        printf("exchange_data() : receive from %s : %s\n", inet_ntoa(clnt_adr.sin_addr), buf);
    }
}

void error_handling(char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}