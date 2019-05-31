#include	"unpxti.h"

int
main(int argc, char **argv)
{
	int				listenfd, connfd, n, flags;
	char			buff[MAXLINE];
	struct pollfd	fds[1];

	if (argc == 2)
		listenfd = Tcp_listen(NULL, argv[1], NULL);
	else if (argc == 3)
		listenfd = Tcp_listen(argv[1], argv[2], NULL);
	else
		err_quit("usage: daytimetcpsrv01 [ <host> ] <service or port>");
	
	connfd = Xti_accept(listenfd, NULL, 0);

	fds[0].fd = connfd;
	fds[0].events = POLLIN | POLLRDNORM | POLLRDBAND | POLLPRI;
	for ( ; ; ) {
		n = poll(fds, 1, INFTIM);
		printf("poll returned %d, revents = 0x%x\n", n, fds[0].revents);

		n = T_rcv(connfd, buff, sizeof(buff), &flags);
		printf("received %d bytes, flags = %d\n", n, flags);
	}
}
