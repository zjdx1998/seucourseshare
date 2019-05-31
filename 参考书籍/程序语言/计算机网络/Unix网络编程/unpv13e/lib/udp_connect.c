/* include udp_connect */
#include	"unp.h"

int
udp_connect(const char *host, const char *serv)
{
	int				sockfd, n;
	struct addrinfo	hints, *res, *ressave;

	bzero(&hints, sizeof(struct addrinfo));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_DGRAM;

	if ( (n = getaddrinfo(host, serv, &hints, &res)) != 0)
		err_quit("udp_connect error for %s, %s: %s",
				 host, serv, gai_strerror(n));
	ressave = res;

	do {
		sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
		if (sockfd < 0)
			continue;	/* ignore this one */

		if (connect(sockfd, res->ai_addr, res->ai_addrlen) == 0)
			break;		/* success */

		Close(sockfd);	/* ignore this one */
	} while ( (res = res->ai_next) != NULL);

	if (res == NULL)	/* errno set from final connect() */
		err_sys("udp_connect error for %s, %s", host, serv);

	freeaddrinfo(ressave);

	return(sockfd);
}
/* end udp_connect */

int
Udp_connect(const char *host, const char *serv)
{
	int		n;

	if ( (n = udp_connect(host, serv)) < 0) {
		err_quit("udp_connect error for %s, %s: %s",
					 host, serv, gai_strerror(-n));
	}
	return(n);
}
