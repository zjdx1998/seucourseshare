#include	"trace.h"

void
traceloop(void)
{
	int					seq, code, done;
	double				rtt;
	struct rec			*rec;
	struct timeval		tvrecv;

	recvfd = Socket(pr->sasend->sa_family, SOCK_RAW, pr->icmpproto);
	setuid(getuid());		/* don't need special permissions anymore */

#ifdef	IPV6
	if (pr->sasend->sa_family == AF_INET6 && verbose == 0) {
		struct icmp6_filter myfilt;
		ICMP6_FILTER_SETBLOCKALL(&myfilt);
		ICMP6_FILTER_SETPASS(ICMP6_TIME_EXCEEDED, &myfilt);
		ICMP6_FILTER_SETPASS(ICMP6_DST_UNREACH, &myfilt);
		setsockopt(recvfd, IPPROTO_IPV6, ICMP6_FILTER,
					&myfilt, sizeof(myfilt));
	}
#endif

	sendfd = Socket(pr->sasend->sa_family, SOCK_DGRAM, 0);

	pr->sabind->sa_family = pr->sasend->sa_family;
	sport = (getpid() & 0xffff) | 0x8000;	/* our source UDP port # */
	sock_set_port(pr->sabind, pr->salen, htons(sport));
	Bind(sendfd, pr->sabind, pr->salen);

	sig_alrm(SIGALRM);

	seq = 0;
	done = 0;
	for (ttl = 1; ttl <= max_ttl && done == 0; ttl++) {
		Setsockopt(sendfd, pr->ttllevel, pr->ttloptname, &ttl, sizeof(int));
		bzero(pr->salast, pr->salen);

		printf("%2d ", ttl);
		fflush(stdout);

		for (probe = 0; probe < nprobes; probe++) {
			rec = (struct rec *) sendbuf;
			rec->rec_seq = ++seq;
			rec->rec_ttl = ttl;
			Gettimeofday(&rec->rec_tv, NULL);

			sock_set_port(pr->sasend, pr->salen, htons(dport + seq));
			Sendto(sendfd, sendbuf, datalen, 0, pr->sasend, pr->salen);

			if ( (code = (*pr->recv)(seq, &tvrecv)) == -3)
				printf(" *");		/* timeout, no reply */
			else {
				char	str[NI_MAXHOST];

				if (sock_cmp_addr(pr->sarecv, pr->salast, pr->salen) != 0) {
					if (getnameinfo(pr->sarecv, pr->salen, str, sizeof(str),
									NULL, 0, 0) == 0)
						printf(" %s (%s)", str,
								Sock_ntop_host(pr->sarecv, pr->salen));
					else
						printf(" %s",
								Sock_ntop_host(pr->sarecv, pr->salen));
					memcpy(pr->salast, pr->sarecv, pr->salen);
				}
				tv_sub(&tvrecv, &rec->rec_tv);
				rtt = tvrecv.tv_sec * 1000.0 + tvrecv.tv_usec / 1000.0;
				printf("  %.3f ms", rtt);

				if (code == -1)		/* port unreachable; at destination */
					done++;
				else if (code >= 0)
					printf(" (ICMP %s)", (*pr->icmpcode)(code));
			}
			fflush(stdout);
		}
		printf("\n");
	}
}
