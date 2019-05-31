#include	"test.h"

/*
 * Fill in the global servaddr{} as a side effect.
 */

int
TcpSockByAddr(char *ipaddr, int port /* host byte order */ )
{
	int					sockfd;

	sockfd = Socket(AF_INET, SOCK_STREAM, 0);

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(port);
	Inet_pton(AF_INET, ipaddr, &servaddr.sin_addr);

	Connect(sockfd, (SA *) &servaddr, sizeof(servaddr));

	return(sockfd);
}

/*
 * Create the default (unconnected) UDP socket.
 * Fill in the global servaddr{} for the caller to use.
 */

int
UdpSockByAddr(char *ipaddr, int port /* host byte order */ )
{
	int					sockfd;

	sockfd = Socket(AF_INET, SOCK_DGRAM, 0);

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(port);
	Inet_pton(AF_INET, ipaddr, &servaddr.sin_addr);

	return(sockfd);
}

/*
 * Create a connected UDP socket.
 */

int
UdpConnSockByAddr(char *ipaddr, int port /* host byte order */ )
{
	int					sockfd;

	sockfd = Socket(AF_INET, SOCK_DGRAM, 0);

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(port);
	Inet_pton(AF_INET, ipaddr, &servaddr.sin_addr);

	Connect(sockfd, (SA *) &servaddr, sizeof(servaddr));

	return(sockfd);
}
