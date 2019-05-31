#include	"ping.h"

void
ping_v4(struct hostent *hptr)
{
	int				size;
	char			recvbuf[BUFSIZE];
	socklen_t		len;
	ssize_t			n;
	struct timeval	tval;

	setuid(getuid());		/* don't need special permissions any more */

	size = 60 * 1024;		/* OK if setsockopt fails */
	setsockopt(sockfd, SOL_SOCKET, SO_RCVBUF, &size, sizeof(size));


	sig_alrm(SIGALRM);		/* send first packet */

	for ( ; ; ) {
		len = salen;
		n = recvfrom(sockfd, recvbuf, sizeof(recvbuf), 0, sarecv, &len);
		if (n < 0) {
			if (errno == EINTR)
				continue;
			else
				err_sys("recvfrom error");
		}

		Gettimeofday(&tval, NULL);
		(*fptrs.proc_f)(recvbuf, n, &tval);
	}
}
