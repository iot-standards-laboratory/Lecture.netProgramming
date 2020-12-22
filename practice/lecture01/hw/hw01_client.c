#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

void error_handling(char *message);

int main(int argc, char *argv[])
{
    int sock;
    int fd;
    char buf;
    struct sockaddr_in serv_addr;
    char message[30];
    int str_len = 0;
    int idx = 0, read_len = 0;

    if (argc != 4)
    {
        printf("Usage : %s <IP> <port>\n", argv[0]);
        exit(1);
    }

    sock = socket(________, ________, ________);    // 완성해보세요
    
    if (sock == -1)
        error_handling("socket() error");

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = ________;    // 완성해보세요
    if (!inet_aton(argv[2], &serv_addr.sin_addr))
    {
        error_handling("Wrong IP is entered!!");
    }
    serv_addr.sin_port = ________(atoi(argv[3])); // 완성해보세요

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1)
        error_handling("connect() error!");

    fd = open(argv[1], ________________); //옵션을 채워보세요.
    if (fd == -1)
        error_handling("open() error!");

    while (read(sock, &buf, sizeof(buf)) > 0)
    {
        write(fd, &buf, sizeof(buf));
    }

    // 소켓을 닫아보세요
    ________________
    
    close(fd);
}

void error_handling(char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}
