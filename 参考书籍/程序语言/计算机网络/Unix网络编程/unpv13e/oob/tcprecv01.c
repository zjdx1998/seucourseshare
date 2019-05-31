#include	"unp.h"

int		listenfd, connfd;

void	sig_urg(int);

int
main(int argc, char **argv)
{
	int		n;
	char	buff[100];

	if (argc == 2)
		listenfd = Tcp_listen(NULL, argv[1], NULL);
	else if (argc == 3)
		listenfd = Tcp_listen(argv[1], argv[2], NULL);
	else
		err_quit("usage: tcprecv01 [ <host> ] <port#>");

	connfd = Accept(listenfd, NULL, NULL);

	Signal(SIGURG, sig_urg);
	Fcntl(connfd, F_SETOWN, getpid());

	for ( ; ; ) {
		if ( (n = Read(connfd, buff, sizeof(buff)-1)) == 0) {
			printf("received EOF\n");
			exit(0);
		}
		buff[n] = 0;	/* null terminate */
		printf("read %d bytes: %s\n", n, buff);
	}
}

void
sig_urg(int signo)
{
	int		n;
	char	buff[100];

	printf("SIGURG received\n");
	n = Recv(connfd, buff, sizeof(buff)-1, MSG_OOB);
	buff[n] = 0;		/* null terminate */
	printf("read %d OOB byte: %s\n", n, buff);
}
