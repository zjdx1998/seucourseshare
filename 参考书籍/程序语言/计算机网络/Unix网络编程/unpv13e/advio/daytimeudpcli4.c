#include	"unp.h"

int
main(int argc, char **argv)
{
	int		sockfd, n, nq;
	char	recvline[MAXLINE + 1];
	socklen_t	len;
	struct sockaddr_storage	ss;

	if (argc != 3)
		err_quit("usage: a.out <hostname or IPaddress> <service or port#>");

	sockfd = Udp_connect(argv[1], argv[2]);

	len = sizeof(ss);
	Getpeername(sockfd, (SA *)&ss, &len);
	printf("connected to %s\n", Sock_ntop_host((SA *)&ss, len));

	Write(sockfd, "", 2);	/* send 1-byte datagram */

	n = Recv(sockfd, recvline, MAXLINE, MSG_PEEK);

	Ioctl(sockfd, FIONREAD, &nq);	/* check FIONREAD support */
	printf("%d bytes from PEEK, %d bytes pending\n", n, nq);

	n = Read(sockfd, recvline, MAXLINE);
	recvline[n] = 0;	/* null terminate */
	Fputs(recvline, stdout);

	exit(0);
}
