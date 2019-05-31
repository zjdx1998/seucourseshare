#include	"trace.h"

struct proto	proto_v4 = { icmpcode_v4, recv_v4, NULL, NULL, NULL, NULL, 0,
							 IPPROTO_ICMP, IPPROTO_IP, IP_TTL };

#ifdef	IPV6
struct proto	proto_v6 = { icmpcode_v6, recv_v6, NULL, NULL, NULL, NULL, 0,
							 IPPROTO_ICMPV6, IPPROTO_IPV6, IPV6_UNICAST_HOPS };
#endif

int		datalen = sizeof(struct rec);	/* defaults */
int		max_ttl = 30;
int		nprobes = 3;
u_short	dport = 32768 + 666;

int
main(int argc, char **argv)
{
	int				c;
	struct addrinfo	*ai;
	char *h;

	opterr = 0;		/* don't want getopt() writing to stderr */
	while ( (c = getopt(argc, argv, "m:v")) != -1) {
		switch (c) {
		case 'm':
			if ( (max_ttl = atoi(optarg)) <= 1)
				err_quit("invalid -m value");
			break;

		case 'v':
			verbose++;
			break;

		case '?':
			err_quit("unrecognized option: %c", c);
		}
	}

	if (optind != argc-1)
		err_quit("usage: traceroute [ -m <maxttl> -v ] <hostname>");
	host = argv[optind];

	pid = getpid();
	Signal(SIGALRM, sig_alrm);

	ai = Host_serv(host, NULL, 0, 0);

	h = Sock_ntop_host(ai->ai_addr, ai->ai_addrlen);
	printf("traceroute to %s (%s): %d hops max, %d data bytes\n",
		   ai->ai_canonname ? ai->ai_canonname : h,
		   h, max_ttl, datalen);

		/* initialize according to protocol */
	if (ai->ai_family == AF_INET) {
		pr = &proto_v4;
#ifdef	IPV6
	} else if (ai->ai_family == AF_INET6) {
		pr = &proto_v6;
		if (IN6_IS_ADDR_V4MAPPED(&(((struct sockaddr_in6 *)ai->ai_addr)->sin6_addr)))
			err_quit("cannot traceroute IPv4-mapped IPv6 address");
#endif
	} else
		err_quit("unknown address family %d", ai->ai_family);

	pr->sasend = ai->ai_addr;		/* contains destination address */
	pr->sarecv = Calloc(1, ai->ai_addrlen);
	pr->salast = Calloc(1, ai->ai_addrlen);
	pr->sabind = Calloc(1, ai->ai_addrlen);
	pr->salen = ai->ai_addrlen;

	traceloop();

	exit(0);
}
