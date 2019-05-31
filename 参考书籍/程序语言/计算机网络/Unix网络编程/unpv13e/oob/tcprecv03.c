#include	"unp.h"

int
main(int argc, char **argv)
{
	int		listenfd, connfd, n, justreadoob = 0;
	char	buff[100];
	fd_set	rset, xset;

	if (argc == 2)
		listenfd = Tcp_listen(NULL, argv[1], NULL);
	else if (argc == 3)
		listenfd = Tcp_listen(argv[1], argv[2], NULL);
	else
		err_quit("usage: tcprecv03 [ <host> ] <port#>");

	connfd = Accept(listenfd, NULL, NULL);

	FD_ZERO(&rset);
	FD_ZERO(&xset);
	for ( ; ; ) {
		FD_SET(connfd, &rset);
		if (justreadoob == 0)
			FD_SET(connfd, &xset);

		Select(connfd + 1, &rset, NULL, &xset, NULL);

		if (FD_ISSET(connfd, &xset)) {
			n = Recv(connfd, buff, sizeof(buff)-1, MSG_OOB);
			buff[n] = 0;		/* null terminate */
			printf("read %d OOB byte: %s\n", n, buff);
			justreadoob = 1;
			FD_CLR(connfd, &xset);
		}

		if (FD_ISSET(connfd, &rset)) {
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
