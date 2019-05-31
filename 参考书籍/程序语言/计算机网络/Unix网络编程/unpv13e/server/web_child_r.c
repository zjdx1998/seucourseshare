#include	"unp.h"
#include	"readline_r.h"

#define	MAXN	16384		/* max #bytes that a client can request */

void
web_child(int sockfd)
{
	int			ntowrite;
	ssize_t		nread;
	char		line[MAXLINE], result[MAXN];
	Rline		rline;

	readline_rinit(sockfd, line, MAXLINE, &rline);
	for ( ; ; ) {
		if ( (nread = Readline_r(&rline)) == 0)
			return;		/* connection closed by other end */

			/* line from client specifies #bytes to write back */
		ntowrite = atol(line);
		if ((ntowrite <= 0) || (ntowrite > MAXN))
			err_quit("client request for %d bytes", ntowrite);

		Writen(sockfd, result, ntowrite);
	}
}
