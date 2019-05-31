#include	"unp.h"
#include	<netinet/tcp.h>		/* for TCP_MAXSEG value */

int
main(int argc, char **argv)
{
	int			sockfd, mss, sendbuff;
	socklen_t	optlen;

	sockfd = Socket(AF_INET, SOCK_STREAM, 0);

		/* Fetch and print the TCP maximum segment size.  */
	optlen = sizeof(mss);
	Getsockopt(sockfd, IPPROTO_TCP, TCP_MAXSEG, &mss, &optlen);
	printf("TCP mss = %d\n", mss);

		/* Set the send buffer size, then fetch it and print its value.  */
	sendbuff = 65536;
	Setsockopt(sockfd, SOL_SOCKET, SO_SNDBUF, &sendbuff, sizeof(sendbuff));

	optlen = sizeof(sendbuff);
	Getsockopt(sockfd, SOL_SOCKET, SO_SNDBUF, &sendbuff, &optlen);
	printf("send buffer size = %d\n", sendbuff);
	exit(0);
}
