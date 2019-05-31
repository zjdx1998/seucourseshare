#include	"unp.h"

int
main(int argc, char **argv)
{
	int		sockfd, n, npend;
	char	recvline[MAXLINE + 1];
	socklen_t	len;
	struct sockaddr_storage	ss;

	if (argc != 3)
		err_quit("usage: a.out <hostname or IPaddress> <service or port#>");

	sockfd = Tcp_connect(argv[1], argv[2]);

	len = sizeof(ss);
	Getpeername(sockfd, (SA *)&ss, &len);
	printf("connected to %s\n", Sock_ntop_host((SA *)&ss, len));

	for ( ; ; ) {
		if ( (n = Recv(sockfd, recvline, MAXLINE, MSG_PEEK)) == 0)
			break;		/* server closed connection */

		Ioctl(sockfd, FIONREAD, &npend);	/* check FIONREAD support */
		printf("%d bytes from PEEK, %d bytes pending\n", n, npend);

		n = Read(sockfd, recvline, MAXLINE);
		recvline[n] = 0;	/* null terminate */
		Fputs(recvline, stdout);
	}
	exit(0);
}
