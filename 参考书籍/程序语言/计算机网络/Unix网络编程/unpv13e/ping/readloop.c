#include	"ping.h"

void
readloop(void)
{
	int				size;
	char			recvbuf[BUFSIZE];
	char			controlbuf[BUFSIZE];
	struct msghdr	msg;
	struct iovec	iov;
	ssize_t			n;
	struct timeval	tval;

	sockfd = Socket(pr->sasend->sa_family, SOCK_RAW, pr->icmpproto);
	setuid(getuid());		/* don't need special permissions any more */
	if (pr->finit)
		(*pr->finit)();

	size = 60 * 1024;		/* OK if setsockopt fails */
	setsockopt(sockfd, SOL_SOCKET, SO_RCVBUF, &size, sizeof(size));

	sig_alrm(SIGALRM);		/* send first packet */

	iov.iov_base = recvbuf;
	iov.iov_len = sizeof(recvbuf);
	msg.msg_name = pr->sarecv;
	msg.msg_iov = &iov;
	msg.msg_iovlen = 1;
	msg.msg_control = controlbuf;
	for ( ; ; ) {
		msg.msg_namelen = pr->salen;
		msg.msg_controllen = sizeof(controlbuf);
		n = recvmsg(sockfd, &msg, 0);
		if (n < 0) {
			if (errno == EINTR)
				continue;
			else
				err_sys("recvmsg error");
		}

		Gettimeofday(&tval, NULL);
		(*pr->fproc)(recvbuf, n, &msg, &tval);
	}
}
