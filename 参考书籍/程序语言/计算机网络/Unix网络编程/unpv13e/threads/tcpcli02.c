#include	"unp.h"

int
main(int argc, char **argv)
{
	int		sockfd;

	if (argc != 3)
		err_quit("usage: tcpcli <hostname> <service>");

	sockfd = Tcp_connect(argv[1], argv[2]);

	str_cli(stdin, sockfd);		/* do it all */

	exit(0);
}
