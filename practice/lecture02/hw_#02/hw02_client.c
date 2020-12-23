#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 70

void error_handling(char *message);
int create_clientsock(char *ip, int port);
void exchange_data(int sock, char *fname);

int main(int argc, char *argv[])
{
    int sock;

    struct sockaddr_in serv_adr;

    if (argc != 4)
    {
        printf("Usage : %s <filename> <IP> <port>\n", argv[0]);
        exit(1);
    }

    sock = create_clientsock(argv[2], atoi(argv[3]));
    exchange_data(sock, argv[1]);

    return 0;
}

void exchange_data(int sock, char *fname)
{
    char buf[BUF_SIZE] = {
        0,
    };
    int file_fd;
    int str_len;
    off_t fsize;

    // file 생성
    file_fd = open(fname, ________); // 완성해보세요
    if (file_fd == -1)
        error_handling("open() error!");

    // file size 수신
    read(sock, &fsize, sizeof(fsize));

    // file 수신
    printf("Start file download [filesize : %d]\n", fsize);
    while (fsize > BUF_SIZE)
    {
        str_len = read(sock, buf, BUF_SIZE);
        write(file_fd, buf, str_len);
        fsize -= str_len;
    }

    str_len = read(sock, buf, fsize);
    write(file_fd, buf, str_len);
    close(file_fd);
    puts("--Complete file download--");

    // Thank you 전송
    str_len = strlen("Thank you");
    write(sock, &str_len, sizeof(int));
    write(sock, "Thank you", str_len);
    puts("Send : Thank you");

    close(file_fd);
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
