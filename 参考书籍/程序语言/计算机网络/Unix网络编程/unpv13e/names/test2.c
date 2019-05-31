#include	"unp.h"

int
main(int argc, char **argv)
{
	int				sockfd, n;
	char			recvline[MAXLINE + 1];
	socklen_t		salen;
	struct sockaddr	*sa, *sabind;

	if (argc != 3)
		err_quit("usage: test2 <hostname/IPaddress> <service/port#>");

	sockfd = Udp_client(argv[1], argv[2], (void **) &sa, &salen);

	/* Same as daytimeudpcli1, but we explicitly bind the wildcard */
	sabind = Malloc(salen);
	bzero(sabind, salen);
	sabind->sa_family = sa->sa_family;
	Bind(sockfd, sabind, salen);
	printf("bound %s\n", Sock_ntop(sabind, salen));

	printf("sending to %s\n", Sock_ntop_host(sa, salen));

	Sendto(sockfd, "", 1, 0, sa, salen);	/* send 1-byte datagram */

	n = Recvfrom(sockfd, recvline, MAXLINE, 0, NULL, NULL);
	recvline[n] = 0;	/* null terminate */
	Fputs(recvline, stdout);

	exit(0);
}
