#include	"unp.h"

int
main(int argc, char **argv)
{
	int					sockfd;
	struct sockaddr_in	servaddr, grpaddr, cliaddr;

	sockfd = Socket(AF_INET, SOCK_DGRAM, 0);

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family      = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port        = htons(SERV_PORT);

	Bind(sockfd, (SA *) &servaddr, sizeof(servaddr));

	bzero(&grpaddr, sizeof(servaddr));
	grpaddr.sin_family      = AF_INET;
	grpaddr.sin_addr.s_addr = inet_addr("224.0.0.1");

	mcast_join(sockfd, &grpaddr, sizeof(grpaddr), NULL, 0);

	dg_echo(sockfd, (SA *) &cliaddr, sizeof(cliaddr));
}
