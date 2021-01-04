#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/time.h>
#include <arpa/inet.h>
#include <netinet/ip.h>      // struct ip*
#include <netinet/ip_icmp.h> // stuct icmp*
#include <netdb.h>           // gethostbyname

#define PACKET_LEN 64
#define BUFFER_SIZE 500
#define MAX_LOOP 5

void make_icmp_packet(struct icmp *icmp, int len, int n);
double tv_sub(struct timeval *out, struct timeval *in);
u_short checksum(u_short *data, int len);
void error_handling(char *buf);

int main(int argc, char *argv[])
{
    int ping_sock; // socket discriptor
    struct hostent *dst_net;
    struct sockaddr_in dst_sa;
    struct ip *ip;
    struct icmp *icmp;
    struct timeval tv;
    char send_buf[PACKET_LEN];
    char recv_buf[BUFFER_SIZE];
    u_int dst_ip;
    int request;
    int rcv_len;
    int cnt = 0;
    char str[32];
    fd_set readfd;
    int i;

    if (argc != 2)
    {
        printf("Usage : %s <Address>\n", argv[0]);
        exit(0);
    }

    // IP 주소면 바로 IP 주소로 출력
    if (inet_addr(argv[1]) != INADDR_NONE)
    {
        printf("PING %s\n", argv[1]);
        dst_ip = inet_addr(argv[1]);
    }
    // 유효하지 않은 IP 주소거나 도메인 네임이면 실행
    else
    {
        if ((dst_net = (gethostbyname(argv[1]))) == NULL)
            error_handling("gethostbyname() error!\n");
        else
        {
            inet_ntop(dst_net->h_addrtype, dst_net->h_addr, str, sizeof(str));
            printf("PING %s\n", str);
            dst_ip = inet_addr(str);
        }
    }

    memset((char *)&________, 0, sizeof(struct ________)); // 1
    dst_sa.sin_family = ________;                          // 1
    dst_sa.sin_addr.s_addr = ________;                     // 1

    ping_sock = socket(________, ________, ________); // 2

    if (ping_sock < 0)
        error_handling("socket() error!\n");

    for (cnt = 0; cnt < MAX_LOOP; cnt++)
    {
        //send icmp packet
        make_icmp_packet((struct icmp *)send_buf, PACKET_LEN, cnt);
        if (sendto(________, (char *)&________, ________, 0, (struct sockaddr *)&________, sizeof(________)) < 0) // 3
            error_handling("sendto() error!\n");

        // read icmp message
        rcv_len = recvfrom(________, ________, ________, 0, NULL, NULL); // 4
        if (rcv_len < 0)
            error_handling("recvfrom() error!\n");

        ip = (struct ip *)recv_buf;
        icmp = (struct icmp *)(recv_buf + (ip->ip_hl * 4));

        // print result
        printf("%d bytes from %s : ", ntohs(ip->ip_len) - ip->ip_hl * 4, inet_ntoa(ip->ip_src));
        printf("icmp_seq : %d ", icmp->________ + 1); // 5
        printf("TTL = %d ", ip->________);            // 6

        gettimeofday(&tv, NULL);
        printf("time = %.3f ms ", tv_sub(&tv, (struct timeval *)icmp->icmp_data));
        printf("\n");

        sleep(1);
    }

    return 0;
}

void make_icmp_packet(struct icmp *icmp, int len, int n)
{
    memset((char *)icmp, 0, len);
    icmp->icmp_type = ICMP_ECHO;
    icmp->icmp_code = 0;
    icmp->icmp_id = getpid();
    icmp->icmp_seq = n;
    gettimeofday((struct timeval *)icmp->icmp_data, NULL);

    icmp->icmp_cksum = 0;
    icmp->icmp_cksum = checksum((u_short *)icmp, len);
}

double tv_sub(struct timeval *out, struct timeval *in)
{
    return (double)out->tv_sec * 1000 + (double)out->tv_usec / 1000 - (double)in->tv_sec * 1000 - (double)in->tv_usec / 1000;
}

u_short checksum(u_short *data, int len)
{
    u_long cksum = 0;
    for (; len > 1; len -= 2)
    {
        cksum += *data++;
        if (cksum & 0x80000000)
            cksum = (cksum & 0xffff) + (cksum >> 16);
    }

    if (len)
    {
        u_short i = 0;
        *(u_char *)(&i) = *(u_char *)data;
        cksum += i;
    }
    while (cksum >> 16)
        cksum = (cksum & 0xffff) + (cksum >> 16);

    return (cksum == 0xffff) ? cksum : ~cksum;
}

void error_handling(char *buf)
{
    fputs(buf, stderr);
    fputc('\n', stderr);
    exit(1);
}