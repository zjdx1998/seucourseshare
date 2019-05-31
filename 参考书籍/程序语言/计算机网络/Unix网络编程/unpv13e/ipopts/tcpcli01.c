#include	"unp.h"

int
main(int argc, char **argv)
{
	int					c, sockfd, len = 0;
	u_char				*ptr = NULL;
	struct addrinfo		*ai;

	if (argc < 2)
		err_quit("usage: tcpcli01 [ -[gG] <hostname> ... ] <hostname>");

	opterr = 0;		/* don't want getopt() writing to stderr */
	while ( (c = getopt(argc, argv, "gG")) != -1) {
		switch (c) {
		case 'g':			/* loose source route */
			if (ptr)
				err_quit("can't use both -g and -G");
			ptr = inet_srcrt_init(0);
			break;

		case 'G':			/* strict source route */
			if (ptr)
				err_quit("can't use both -g and -G");
			ptr = inet_srcrt_init(1);
			break;

		case '?':
			err_quit("unrecognized option: %c", c);
		}
	}

	if (ptr)
		while (optind < argc-1)
			len = inet_srcrt_add(argv[optind++]);
	else
		if (optind < argc-1)
			err_quit("need -g or -G to specify route");

	if (optind != argc-1)
		err_quit("missing <hostname>");

	ai = Host_serv(argv[optind], SERV_PORT_STR, AF_INET, SOCK_STREAM);

	sockfd = Socket(ai->ai_family, ai->ai_socktype, ai->ai_protocol);

	if (ptr) {
		len = inet_srcrt_add(argv[optind]);	/* dest at end */
		Setsockopt(sockfd, IPPROTO_IP, IP_OPTIONS, ptr, len);
		free(ptr);
	}

	Connect(sockfd, ai->ai_addr, ai->ai_addrlen);

	str_cli(stdin, sockfd);		/* do it all */

	exit(0);
}
