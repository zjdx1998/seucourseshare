#include	"unp.h"

#define	SAP_NAME	"sap.mcast.net"	/* default group name and port */
#define	SAP_PORT	"9875"

void	loop(int, socklen_t);

int
main(int argc, char **argv)
{
	int					sockfd;
	const int			on = 1;
	socklen_t			salen;
	struct sockaddr		*sa;

	if (argc == 1)
		sockfd = Udp_client(SAP_NAME, SAP_PORT, (void **) &sa, &salen);
	else if (argc == 4)
		sockfd = Udp_client(argv[1], argv[2], (void **) &sa, &salen);
	else
		err_quit("usage: mysdr <mcast-addr> <port#> <interface-name>");

	Setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
	Bind(sockfd, sa, salen);

	Mcast_join(sockfd, sa, salen, (argc == 4) ? argv[3] : NULL, 0);

	loop(sockfd, salen);	/* receive and print */

	exit(0);
}
