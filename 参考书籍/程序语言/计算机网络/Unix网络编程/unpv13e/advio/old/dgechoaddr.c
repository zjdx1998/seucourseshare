#include	"unp.h"

#undef	MAXLINE
#define	MAXLINE	20		/* to see datagram truncation */

void
dg_echo(int sockfd, SA *pcliaddr, socklen_t clilen)
{
	int				flags;
	const int		on = 1;
	socklen_t		len;
	ssize_t			n;
	char			mesg[MAXLINE], str[INET6_ADDRSTRLEN];
	struct in_addr	dstaddr;

#ifdef	IP_RECVDSTADDR
	Setsockopt(sockfd, IPPROTO_IP, IP_RECVDSTADDR, &on, sizeof(on));
#endif

	for ( ; ; ) {
		len = clilen;
		flags = 0;
		n = Recvfrom_flags(sockfd, mesg, MAXLINE, &flags,
						   pcliaddr, &len, &dstaddr);
		printf("%d-byte datagram from %s", n, Sock_ntop(pcliaddr, len));
		if ((flags & MSG_CTRUNC) == 0)
			printf(", to %s", Inet_ntop(AF_INET, &dstaddr, str, sizeof(str)));
#ifdef	MSG_TRUNC
		if (flags & MSG_TRUNC)	printf(" (datagram truncated)");
#endif
#ifdef	MSG_BCAST
		if (flags & MSG_BCAST)	printf(" (broadcast)");
#endif
#ifdef	MSG_MCAST
		if (flags & MSG_MCAST)	printf(" (multicast)");
#endif
		printf("\n");

		Sendto(sockfd, mesg, n, 0, pcliaddr, clilen);
	}
}
