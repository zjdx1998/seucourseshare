#include	"trace.h"

extern int gotalarm;

/*
 * Return: -3 on timeout
 *		   -2 on ICMP time exceeded in transit (caller keeps going)
 *		   -1 on ICMP port unreachable (caller is done)
 *		 >= 0 return value is some other ICMP unreachable code
 */

int
recv_v4(int seq, struct timeval *tv)
{
	int				hlen1, hlen2, icmplen, ret;
	socklen_t		len;
	ssize_t			n;
	struct ip		*ip, *hip;
	struct icmp		*icmp;
	struct udphdr	*udp;

	gotalarm = 0;
	alarm(3);
	for ( ; ; ) {
		if (gotalarm)
			return(-3);		/* alarm expired */
		len = pr->salen;
		n = recvfrom(recvfd, recvbuf, sizeof(recvbuf), 0, pr->sarecv, &len);
		if (n < 0) {
			if (errno == EINTR)
				continue;
			else
				err_sys("recvfrom error");
		}

		ip = (struct ip *) recvbuf;	/* start of IP header */
		hlen1 = ip->ip_hl << 2;		/* length of IP header */
	
		icmp = (struct icmp *) (recvbuf + hlen1); /* start of ICMP header */
		if ( (icmplen = n - hlen1) < 8)
			continue;				/* not enough to look at ICMP header */
	
		if (icmp->icmp_type == ICMP_TIMXCEED &&
			icmp->icmp_code == ICMP_TIMXCEED_INTRANS) {
			if (icmplen < 8 + sizeof(struct ip))
				continue;			/* not enough data to look at inner IP */

			hip = (struct ip *) (recvbuf + hlen1 + 8);
			hlen2 = hip->ip_hl << 2;
			if (icmplen < 8 + hlen2 + 4)
				continue;			/* not enough data to look at UDP ports */

			udp = (struct udphdr *) (recvbuf + hlen1 + 8 + hlen2);
 			if (hip->ip_p == IPPROTO_UDP &&
				udp->uh_sport == htons(sport) &&
				udp->uh_dport == htons(dport + seq)) {
				ret = -2;		/* we hit an intermediate router */
				break;
			}

		} else if (icmp->icmp_type == ICMP_UNREACH) {
			if (icmplen < 8 + sizeof(struct ip))
				continue;			/* not enough data to look at inner IP */

			hip = (struct ip *) (recvbuf + hlen1 + 8);
			hlen2 = hip->ip_hl << 2;
			if (icmplen < 8 + hlen2 + 4)
				continue;			/* not enough data to look at UDP ports */

			udp = (struct udphdr *) (recvbuf + hlen1 + 8 + hlen2);
 			if (hip->ip_p == IPPROTO_UDP &&
				udp->uh_sport == htons(sport) &&
				udp->uh_dport == htons(dport + seq)) {
				if (icmp->icmp_code == ICMP_UNREACH_PORT)
					ret = -1;	/* have reached destination */
				else
					ret = icmp->icmp_code;	/* 0, 1, 2, ... */
				break;
			}
		}
		if (verbose) {
			printf(" (from %s: type = %d, code = %d)\n",
					Sock_ntop_host(pr->sarecv, pr->salen),
					icmp->icmp_type, icmp->icmp_code);
		}
		/* Some other ICMP error, recvfrom() again */
	}
	alarm(0);					/* don't leave alarm running */
	Gettimeofday(tv, NULL);		/* get time of packet arrival */
	return(ret);
}
