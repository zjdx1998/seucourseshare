#include	"unp.h"

int		listenfd, connfd;

void	sig_urg(int);

int
main(int argc, char **argv)
{
	int		size;

	if (argc == 2)
		listenfd = Tcp_listen(NULL, argv[1], NULL);
	else if (argc == 3)
		listenfd = Tcp_listen(argv[1], argv[2], NULL);
	else
		err_quit("usage: tcprecv05 [ <host> ] <port#>");

	size = 4096;
	Setsockopt(listenfd, SOL_SOCKET, SO_RCVBUF, &size, sizeof(size));

	connfd = Accept(listenfd, NULL, NULL);

	Signal(SIGURG, sig_urg);
	Fcntl(connfd, F_SETOWN, getpid());

	for ( ; ; )
		pause();
}

void
sig_urg(int signo)
{
	int		n;
	char	buff[2048];

	printf("SIGURG received\n");
	n = Recv(connfd, buff, sizeof(buff)-1, MSG_OOB);
	buff[n] = 0;		/* null terminate */
	printf("read %d OOB byte\n", n);
}
