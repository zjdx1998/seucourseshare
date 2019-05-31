#include	"unp.h"

int
main(int argc, char **argv)
{
	int		sockfd, size;
	char	buff[16384];

	if (argc != 3)
		err_quit("usage: tcpsend05 <host> <port#>");

	sockfd = Tcp_connect(argv[1], argv[2]);

	size = 32768;
	Setsockopt(sockfd, SOL_SOCKET, SO_SNDBUF, &size, sizeof(size));

	Write(sockfd, buff, 16384);
	printf("wrote 16384 bytes of normal data\n");
	sleep(5);

	Send(sockfd, "a", 1, MSG_OOB);
	printf("wrote 1 byte of OOB data\n");

	Write(sockfd, buff, 1024);
	printf("wrote 1024 bytes of normal data\n");

	exit(0);
}
