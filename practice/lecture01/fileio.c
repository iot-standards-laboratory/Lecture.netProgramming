#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

void error_handling(char *message);
#define BUF_SIZE 100

void writeFile();
void readFile();
void error_handling(char *message);

int main(void)
{
    writeFile();
    readFile();
}

void writeFile()
{
    int fd;
    char buf[] = "Let's go!\n";

    fd = open("data.txt", O_CREAT | O_WRONLY | O_TRUNC);
    if (fd == -1)
        error_handling("open() error!");

    printf("file descriptor: %d \n", fd);

    if (write(fd, buf, sizeof(buf) - 1) == -1)
        error_handling("write() error!");

    close(fd);
}

void readFile()
{
    int fd;
    char buf[100];

    fd = open("data.txt", O_RDONLY);
    if (fd == -1)
        error_handling("open() error!");

    printf("file descriptor: %d \n", fd);

    if (read(fd, buf, sizeof(buf)) == -1)
        error_handling("read() error!");

    printf("file data: %s", buf);

    close(fd);
}

void error_handling(char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}

/*
root@com:/home/swyoon/tcpip# gcc low_open.c -o lopen
root@com:/home/swyoon/tcpip# ./lopen
file descriptor: 3 
root@com:/home/swyoon/tcpip# cat data.txt
Let's go!
root@com:/home/swyoon/tcpip# 
*/
