#include	"unp.h"

void
str_echo(int sockfd)
{
	ssize_t		n;
	char		line[MAXLINE];

	heartbeat_serv(sockfd, 1, 5);

	for ( ; ; ) {
		if ( (n = Readline(sockfd, line, MAXLINE)) == 0)
			return;		/* connection closed by other end */

		Writen(sockfd, line, n);
	}
}
