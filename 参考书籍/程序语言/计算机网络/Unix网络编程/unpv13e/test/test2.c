#include	"unp.h"

int
main(int argc, char **argv)
{
	int				sockfd;
	socklen_t		salen;
	struct addrinfo	*res;
	struct sockaddr	*cli, *serv;

	if (argc != 2)
		err_quit("usage: test2 <IPaddress>");

#ifdef	notdef
	res = Host_serv(argv[1], "daytime", AF_UNSPEC, SOCK_DGRAM);
	printf("res->ai_addrlen = %d\n", res->ai_addrlen);
	printf("res->ai_addr = %p\n", res->ai_addr);
	printf("res->ai_next = %p\n", res->ai_next);
	printf("res->ai_addr->sa_family = %p\n", res->ai_addr->sa_family);
#endif
	sockfd = Udp_client(argv[1], "13", (void **) &serv, &salen);
	printf("sockfd = %d\n", sockfd);

	exit(0);
}
#ifdef	notdef
int
udp_client(const char *host, const char *serv, void **saptr, socklen_t *lenp)
{
	int				sockfd, n;
	struct addrinfo	hints, *res, *ressave;

	bzero(&hints, sizeof(struct addrinfo));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_DGRAM;

	if ( (n = getaddrinfo(host, serv, &hints, &res)) != 0)
		err_quit("udp_client error for %s, %s: %s",
				 host, serv, gai_strerror(n));
	ressave = res;

	do {
		sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
		if (sockfd >= 0)
			break;		/* success */
	} while ( (res = res->ai_next) != NULL);

	if (res == NULL)	/* errno set from final socket() */
		err_sys("udp_client error for %s, %s", host, serv);

	*saptr = Malloc(res->ai_addrlen);
	memcpy(*saptr, res->ai_addr, res->ai_addrlen);
	*lenp = res->ai_addrlen;

	freeaddrinfo(ressave);

	return(sockfd);
}

int
Udp_client(const char *host, const char *serv, void **saptr, socklen_t *lenptr)
{
	return(udp_client(host, serv, saptr, lenptr));
}
#endif
