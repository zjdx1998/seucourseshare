#include	"unp.h"

int
main(int argc, char **argv)
{
	int					sockfd;
	struct sockaddr_in6	servaddr, cliaddr;

	sockfd = Socket(AF_INET6, SOCK_DGRAM, 0);

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin6_family     = AF_INET6;
	servaddr.sin6_addr       = in6addr_any;
	servaddr.sin6_port       = htons(SERV_PORT);

	Bind(sockfd, (SA *) &servaddr, sizeof(servaddr));

	dg_echo(sockfd, (SA *) &cliaddr, sizeof(cliaddr));
}
