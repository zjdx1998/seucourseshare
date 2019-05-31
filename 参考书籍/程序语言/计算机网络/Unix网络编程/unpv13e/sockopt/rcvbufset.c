#include	"unp.h"
#include	<netinet/tcp.h>		/* for TCP_MAXSEG */

int
main(int argc, char **argv)
{
	int					sockfd, rcvbuf, mss;
	socklen_t			len;
	struct sockaddr_in	servaddr;

	if (argc != 2)
		err_quit("usage: rcvbufset <IPaddress>");

	sockfd = Socket(AF_INET, SOCK_STREAM, 0);

	len = sizeof(rcvbuf);
	Getsockopt(sockfd, SOL_SOCKET, SO_RCVBUF, &rcvbuf, &len);
	len = sizeof(mss);
	Getsockopt(sockfd, IPPROTO_TCP, TCP_MAXSEG, &mss, &len);
	printf("defaults: SO_RCVBUF = %d, MSS = %d\n", rcvbuf, mss);

	rcvbuf = 9973;		/* a prime number */
	Setsockopt(sockfd, SOL_SOCKET, SO_RCVBUF, &rcvbuf, sizeof(rcvbuf));
	len = sizeof(rcvbuf);
	Getsockopt(sockfd, SOL_SOCKET, SO_RCVBUF, &rcvbuf, &len);
	printf("SO_RCVBUF = %d (after setting it to 9973)\n", rcvbuf);

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(13);		/* daytime server */
	Inet_pton(AF_INET, argv[1], &servaddr.sin_addr);

	Connect(sockfd, (SA *) &servaddr, sizeof(servaddr));

	len = sizeof(rcvbuf);
	Getsockopt(sockfd, SOL_SOCKET, SO_RCVBUF, &rcvbuf, &len);
	len = sizeof(mss);
	Getsockopt(sockfd, IPPROTO_TCP, TCP_MAXSEG, &mss, &len);
	printf("after connect: SO_RCVBUF = %d, MSS = %d\n", rcvbuf, mss);


	 exit(0);
}
