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
	int fd;
	int serv_sock;
	int clnt_sock;
	char buf;
	struct sockaddr_in serv_addr;
	struct sockaddr_in clnt_addr;
	socklen_t clnt_addr_size;

	char message[] = "Hello World!";

	if (argc != 3)
	{
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}

	serv_sock = socket(________, ________, ________); // 완성해보세요
	if (serv_sock == -1)
		error_handling("socket() error");

	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = ________;	// 완성해보세요
	serv_addr.sin_addr.s_addr = ________;	// 완성해보세요
	serv_addr.sin_port = htons(atoi(argv[2]));

	if (bind(________, ________, ________)) == -1)	// 완성해보세요
		error_handling("bind() error");

	if (listen(________, ________) == -1)	// 완성해보세요
		error_handling("listen() error");

	clnt_addr_size = sizeof(clnt_addr);
	clnt_sock = accept(serv_sock, (struct sockaddr *)&clnt_addr, &clnt_addr_size);
	if (clnt_sock == -1)
		error_handling("accept() error");

	fd = open(argv[1], ________); //옵션을 채워보세요
	if (fd == -1)
		error_handling("open() error!");

	while (read(________, &buf, sizeof(buf)) > 0) // 완성하세요
	{
		write(________, &buf, sizeof(buf)); // 완성하세요
	}

	// 소켓을 닫아보세요
	________________
	________________
	
	close(fd);
}

void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
