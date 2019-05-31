#include	"unp.h"

int
main(int argc, char **argv)
{
	int		listenfd, connfd, n;
	char	buff[100];
	fd_set	rset, xset;

	if (argc == 2)
		listenfd = Tcp_listen(NULL, argv[1], NULL);
	else if (argc == 3)
		listenfd = Tcp_listen(argv[1], argv[2], NULL);
	else
		err_quit("usage: tcprecv02 [ <host> ] <port#>");

	connfd = Accept(listenfd, NULL, NULL);

	FD_ZERO(&rset);
	FD_ZERO(&xset);
	for ( ; ; ) {
		FD_SET(connfd, &rset);
		FD_SET(connfd, &xset);

		Select(connfd + 1, &rset, NULL, &xset, NULL);

		if (FD_ISSET(connfd, &xset)) {
			n = Recv(connfd, buff, sizeof(buff)-1, MSG_OOB);
			buff[n] = 0;		/* null terminate */
			printf("read %d OOB byte: %s\n", n, buff);
		}

		if (FD_ISSET(connfd, &rset)) {
			if ( (n = Read(connfd, buff, sizeof(buff)-1)) == 0) {
				printf("received EOF\n");
				exit(0);
			}
			buff[n] = 0;	/* null terminate */
			printf("read %d bytes: %s\n", n, buff);
		}
	}
}
