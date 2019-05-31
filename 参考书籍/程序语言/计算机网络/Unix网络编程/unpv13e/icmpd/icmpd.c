/* include icmpd1 */
#include	"icmpd.h"

int
main(int argc, char **argv)
{
	int		i, sockfd;
	struct sockaddr_un sun;

	if (argc != 1)
		err_quit("usage: icmpd");

	maxi = -1;					/* index into client[] array */
	for (i = 0; i < FD_SETSIZE; i++)
		client[i].connfd = -1;	/* -1 indicates available entry */
	FD_ZERO(&allset);

	fd4 = Socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
	FD_SET(fd4, &allset);
	maxfd = fd4;

#ifdef	IPV6
	fd6 = Socket(AF_INET6, SOCK_RAW, IPPROTO_ICMPV6);
	FD_SET(fd6, &allset);
	maxfd = max(maxfd, fd6);
#endif

	listenfd = Socket(AF_UNIX, SOCK_STREAM, 0);
	sun.sun_family = AF_LOCAL;
	strcpy(sun.sun_path, ICMPD_PATH);
	unlink(ICMPD_PATH);
	Bind(listenfd, (SA *)&sun, sizeof(sun));
	Listen(listenfd, LISTENQ);
	FD_SET(listenfd, &allset);
	maxfd = max(maxfd, listenfd);
/* end icmpd1 */

/* include icmpd2 */
	for ( ; ; ) {
		rset = allset;
		nready = Select(maxfd+1, &rset, NULL, NULL, NULL);

		if (FD_ISSET(listenfd, &rset))
			if (readable_listen() <= 0)
				continue;

		if (FD_ISSET(fd4, &rset))
			if (readable_v4() <= 0)
				continue;

#ifdef	IPV6
		if (FD_ISSET(fd6, &rset))
			if (readable_v6() <= 0)
				continue;
#endif

		for (i = 0; i <= maxi; i++) {	/* check all clients for data */
			if ( (sockfd = client[i].connfd) < 0)
				continue;
			if (FD_ISSET(sockfd, &rset))
				if (readable_conn(i) <= 0)
					break;				/* no more readable descriptors */
		}
	}
	exit(0);
}
/* end icmpd2 */
