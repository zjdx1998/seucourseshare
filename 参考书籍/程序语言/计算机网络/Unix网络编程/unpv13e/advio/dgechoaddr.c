#include	"unpifi.h"

#undef	MAXLINE
#define	MAXLINE	20		/* to see datagram truncation */

void
dg_echo(int sockfd, SA *pcliaddr, socklen_t clilen)
{
	int						flags;
	const int				on = 1;
	socklen_t				len;
	ssize_t					n;
	char					mesg[MAXLINE], str[INET6_ADDRSTRLEN],
							ifname[IFNAMSIZ];
	struct in_addr			in_zero;
	struct unp_in_pktinfo	pktinfo;

#ifdef	IP_RECVDSTADDR
	if (setsockopt(sockfd, IPPROTO_IP, IP_RECVDSTADDR, &on, sizeof(on)) < 0)
		err_ret("setsockopt of IP_RECVDSTADDR");
#endif
#ifdef	IP_RECVIF
	if (setsockopt(sockfd, IPPROTO_IP, IP_RECVIF, &on, sizeof(on)) < 0)
		err_ret("setsockopt of IP_RECVIF");
#endif
	bzero(&in_zero, sizeof(struct in_addr));	/* all 0 IPv4 address */

	for ( ; ; ) {
		len = clilen;
		flags = 0;
		n = Recvfrom_flags(sockfd, mesg, MAXLINE, &flags,
						   pcliaddr, &len, &pktinfo);
		printf("%d-byte datagram from %s", n, Sock_ntop(pcliaddr, len));
		if (memcmp(&pktinfo.ipi_addr, &in_zero, sizeof(in_zero)) != 0)
			printf(", to %s", Inet_ntop(AF_INET, &pktinfo.ipi_addr,
										str, sizeof(str)));
		if (pktinfo.ipi_ifindex > 0)
			printf(", recv i/f = %s",
				   If_indextoname(pktinfo.ipi_ifindex, ifname));
#ifdef	MSG_TRUNC
		if (flags & MSG_TRUNC)	printf(" (datagram truncated)");
#endif
#ifdef	MSG_CTRUNC
		if (flags & MSG_CTRUNC)	printf(" (control info truncated)");
#endif
#ifdef	MSG_BCAST
		if (flags & MSG_BCAST)	printf(" (broadcast)");
#endif
#ifdef	MSG_MCAST
		if (flags & MSG_MCAST)	printf(" (multicast)");
#endif
		printf("\n");

		Sendto(sockfd, mesg, n, 0, pcliaddr, len);
	}
}
