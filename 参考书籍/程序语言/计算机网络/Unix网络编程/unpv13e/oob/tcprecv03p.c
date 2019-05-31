#include	"unp.h"

int
main(int argc, char **argv)
{
	int		listenfd, connfd, n, justreadoob = 0;
	char	buff[100];
	struct pollfd	pollfd[1];

	if (argc == 2)
		listenfd = Tcp_listen(NULL, argv[1], NULL);
	else if (argc == 3)
		listenfd = Tcp_listen(argv[1], argv[2], NULL);
	else
		err_quit("usage: tcprecv03p [ <host> ] <port#>");

	connfd = Accept(listenfd, NULL, NULL);

	pollfd[0].fd = connfd;
	pollfd[0].events = POLLRDNORM;
	for ( ; ; ) {
		if (justreadoob == 0)
			pollfd[0].events |= POLLRDBAND;

		Poll(pollfd, 1, INFTIM);

		if (pollfd[0].revents & POLLRDBAND) {
			n = Recv(connfd, buff, sizeof(buff)-1, MSG_OOB);
			buff[n] = 0;		/* null terminate */
			printf("read %d OOB byte: %s\n", n, buff);
			justreadoob = 1;
			pollfd[0].events &= ~POLLRDBAND;	/* turn bit off */
		}

		if (pollfd[0].revents & POLLRDNORM) {
			if ( (n = Read(connfd, buff, sizeof(buff)-1)) == 0) {
				printf("received EOF\n");
				exit(0);
			}
			buff[n] = 0;	/* null terminate */
			printf("read %d bytes: %s\n", n, buff);
			justreadoob = 0;
		}
	}
}
