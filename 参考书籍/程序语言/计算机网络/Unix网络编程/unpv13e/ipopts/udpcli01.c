#include	"unp.h"

int
main(int argc, char **argv)
{
	int					c, sockfd, len = 0;
	u_char				*ptr = NULL;
	void				*rth;
	struct addrinfo		*ai;

	if (argc < 2)
		err_quit("usage: udpcli01 [ <hostname> ... ] <hostname>");

	if (argc > 2) {
		int i;

		len = Inet6_rth_space(IPV6_RTHDR_TYPE_0, argc-2);
		ptr = Malloc(len);
		Inet6_rth_init(ptr, len, IPV6_RTHDR_TYPE_0, argc-2);
		for (i = 1; i < argc-1; i++) {
			ai = Host_serv(argv[i], NULL, AF_INET6, 0);
			Inet6_rth_add(ptr, &((struct sockaddr_in6 *)ai->ai_addr)->sin6_addr);
		}
	}

	ai = Host_serv(argv[argc-1], SERV_PORT_STR, AF_INET6, SOCK_DGRAM);

	sockfd = Socket(ai->ai_family, ai->ai_socktype, ai->ai_protocol);

	if (ptr) {
		Setsockopt(sockfd, IPPROTO_IPV6, IPV6_RTHDR, ptr, len);
		free(ptr);
	}

	dg_cli(stdin, sockfd, ai->ai_addr, ai->ai_addrlen);	/* do it all */

	exit(0);
}
