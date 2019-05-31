#include	"unp.h"

int
main(int argc, char **argv)
{
	int		sockfd, n;
	char	recvline[MAXLINE + 1];

	if (argc != 3)
		err_quit("usage: daytimeudpcli2 <hostname/IPaddress> <service/port#>");

	sockfd = Udp_connect(argv[1], argv[2]);

	Write(sockfd, "", 1);	/* send 1-byte datagram */

	n = Read(sockfd, recvline, MAXLINE);
	recvline[n] = '\0';	/* null terminate */
	Fputs(recvline, stdout);

	exit(0);
}
