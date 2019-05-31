#include	"unp.h"

static void	recvfrom_int(int);
static int	count;

void
dg_echo(int sockfd, SA *pcliaddr, socklen_t clilen)
{
	int			n;
	socklen_t	len;
	char		mesg[MAXLINE];

	Signal(SIGINT, recvfrom_int);

	n = 220 * 1024;
	Setsockopt(sockfd, SOL_SOCKET, SO_RCVBUF, &n, sizeof(n));

	for ( ; ; ) {
		len = clilen;
		Recvfrom(sockfd, mesg, MAXLINE, 0, pcliaddr, &len);

		count++;
	}
}

static void
recvfrom_int(int signo)
{
	printf("\nreceived %d datagrams\n", count);
	exit(0);
}
