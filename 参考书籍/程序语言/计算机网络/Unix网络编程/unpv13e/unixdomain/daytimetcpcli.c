#include	"unp.h"

int
main(int argc, char **argv)
{
	int				sockfd, n;
	char			recvline[MAXLINE + 1];
	socklen_t		len;
	struct sockaddr	*sa;

	if (argc != 3)
		err_quit("usage: daytimetcpcli <hostname/IPaddress> <service/port#>");

	sockfd = Tcp_connect(argv[1], argv[2]);

	sa = Malloc(sizeof(struct sockaddr_storage));
	len = sizeof(struct sockaddr_storage);
	Getpeername(sockfd, sa, &len);
	printf("connected to %s\n", Sock_ntop_host(sa, len));
	sleep(5);

	while ( (n = Read(sockfd, recvline, MAXLINE)) > 0) {
		recvline[n] = 0;	/* null terminate */
		printf("%d bytes: %s", n, recvline);
	}
	exit(0);
}
