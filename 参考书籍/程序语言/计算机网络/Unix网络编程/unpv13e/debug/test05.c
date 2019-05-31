#include	"unpxti.h"

int		listenfd, connfd;

void
sig_poll(int signo)
{
	int		n, flags;
	char	buff[MAXLINE];

	printf("SIGPOLL, event = %d\n", t_look(connfd));
	n = T_rcv(connfd, buff, sizeof(buff), &flags);
	printf("received %d bytes, flags = %d\n", n, flags);
}

int
main(int argc, char **argv)
{
	if (argc == 2)
		listenfd = Tcp_listen(NULL, argv[1], NULL);
	else if (argc == 3)
		listenfd = Tcp_listen(argv[1], argv[2], NULL);
	else
		err_quit("usage: test05 [ <host> ] <service or port>");
	
	connfd = Xti_accept(listenfd, NULL, 0);

	Signal(SIGPOLL, sig_poll);
	Ioctl(connfd, I_SETSIG, S_RDNORM);

	for ( ; ; ) {
		pause();
	}
}
