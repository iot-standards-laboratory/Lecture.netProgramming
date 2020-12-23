#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/stat.h>

#define BUF_SIZE 1000

void error_handling(char *message);
int create_serversock(int port);
int create_clientsock(int serv_sock);
void exchange_data(int clnt_sock, char *fname);
off_t getFileSize(char *fname);

int main(int argc, char *argv[])
{

	int serv_sock, clnt_sock;
	int i;

	if (argc != 3)
	{
		printf("Usage : %s <filename> <port>\n", argv[0]);
		exit(1);
	}

	serv_sock = create_serversock(atoi(argv[2]));

	for (i = 0; i < 5; i++)
	{
		clnt_sock = create_clientsock(serv_sock);
		if (clnt_sock == -1)
			error_handling("accept() error");
		else
			printf("Connected client %d \n", i + 1);

		exchange_data(clnt_sock, argv[1]);
	}

	close(serv_sock);
	return 0;
}

int create_serversock(int port)
{
	int serv_sock, clnt_sock;
	int str_len, i;

	struct sockaddr_in serv_adr;

	serv_sock = socket(________, ________, ________); // TCP/IPv4 서버 소켓을 생성해보세요
	if (serv_sock == -1)
		error_handling("socket() error");

	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_addr.s_addr = ________; // 완성해보세요
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
	clnt_sock = accept(serv_sock, ________, ________); // 완성해보세요

	return clnt_sock;
}

void exchange_data(int clnt_sock, char *fname)
{
	int str_len;
	char buf[BUF_SIZE] = {
		0,
	};
	int file_fd = -1;
	off_t fsize = getFileSize(fname);

	// file size 송신
	write(clnt_sock, &fsize, sizeof(fsize));

	// file 송신
	file_fd = open(fname, O_RDONLY);
	if (file_fd == -1)
		error_handling("open() error!");

	while (fsize > BUF_SIZE)
	{
		str_len = read(________, buf, BUF_SIZE); // 완성해보세요
		write(________, buf, str_len);			 // 완성해보세요
		fsize -= str_len;
	}

	str_len = read(________, buf, fsize); // 완성해보세요
	write(________, buf, str_len);		  // 완성해보세요

	// Thank you 수신
	read(clnt_sock, &str_len, sizeof(int));
	str_len = read(clnt_sock, buf, str_len);
	buf[str_len] = 0;
	puts(buf); // Thank you 출력

	close(file_fd);
	close(clnt_sock);
}

void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}

off_t getFileSize(char *fname)
{
	struct stat st;
	if (stat(fname, &st) == -1)
		return -1;

	return st.st_size;
}