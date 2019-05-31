#include	"unp.h"

int
main(int argc, char **argv)
{
	int					sockfd;
	struct linger		ling;
	struct sockaddr_in	servaddr;

	if (argc != 2)
		err_quit("usage: tcpcli <IPaddress>");

	sockfd = Socket(AF_INET, SOCK_STREAM, 0);

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(SERV_PORT);
	Inet_pton(AF_INET, argv[1], &servaddr.sin_addr);

	Connect(sockfd, (SA *) &servaddr, sizeof(servaddr));

	ling.l_onoff = 1;		/* cause RST to be sent on close() */
	ling.l_linger = 0;
	Setsockopt(sockfd, SOL_SOCKET, SO_LINGER, &ling, sizeof(ling));
	Close(sockfd);

	exit(0);
}
