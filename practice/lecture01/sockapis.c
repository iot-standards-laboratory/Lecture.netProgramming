#include <stdio.h>
#include <arpa/inet.h>
#include <stdlib.h>

void error_handling(char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}

void ex_hton()
{
    // htons와 htonl : 핵심 개념은 Little Endian -> Big Endian
    // IBM 계열의 PC를 제외한 나머지는 Little Endian이라고 볼 수 있음
    // Network는 Big Endian을 사용하기 때문에 htons 또는 htonl 을 통해 값이 변함을 확인할 수 있음
    unsigned short host_port = 0x1234;
    unsigned short net_port;
    unsigned long host_addr = 0x12345678;
    unsigned long net_addr;

    net_port = htons(host_port);
    net_addr = htonl(host_addr);

    printf("Host ordered port: %#x \n", host_port);
    printf("Network ordered port: %#x \n", net_port);
    printf("Host ordered address: %#lx \n", host_addr);
    printf("Network ordered address: %#lx \n", net_addr);
}

void ex_inet()
{
    char *addr1 = "127.212.124.78";
    char *addr2 = "127.212.124.256";
    struct sockaddr_in addr_inet;

    if (!inet_aton(addr1, &addr_inet.sin_addr))
        error_handling("Conversion error");
    else
        printf("Network ordered integer addr: %#x \n", addr_inet.sin_addr.s_addr);

    if (!inet_aton(addr2, &addr_inet.sin_addr))
        error_handling("Conversion error");
    else
        printf("Network ordered integer addr: %#x \n", addr_inet.sin_addr.s_addr);

    addr_inet.sin_addr.s_addr = inet_addr(addr1);
    if (addr_inet.sin_addr.s_addr == INADDR_NONE)
        printf("Error occured! \n");
    else
        printf("Network ordered integer addr: %#lx \n", addr_inet.sin_addr.s_addr);

    addr_inet.sin_addr.s_addr = inet_addr(addr2);
    if (addr_inet.sin_addr.s_addr == INADDR_NONE)
        printf("Error occureded \n");
    else
        printf("Network ordered integer addr: %#lx \n\n", addr_inet.sin_addr.s_addr);
}

int main(int argc, char *argv[])
{
    ex_hton();
    ex_inet();
}