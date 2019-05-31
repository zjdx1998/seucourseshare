#include	"unp.h"
#include	<time.h>

int
main(int argc, char **argv)
{
	int				listenfd, connfd;
	socklen_t		addrlen, len;
	struct sockaddr	*cliaddr;
	struct linger	ling;
	char			buff[MAXLINE];
	time_t			ticks;

	if (argc == 2)
		listenfd = Tcp_listen(NULL, argv[1], &addrlen);
	else if (argc == 3)
		listenfd = Tcp_listen(argv[1], argv[2], &addrlen);
	else
		err_quit("usage: daytimetcpsrv3 [ <host> ] <service or port>");

	cliaddr = Malloc(addrlen);

	for ( ; ; ) {
		len = addrlen;
		connfd = Accept(listenfd, cliaddr, &len);
		printf("connection from %s\n", Sock_ntop(cliaddr, len));

			/* force RST instead of FIN after data */
		ling.l_onoff = 1;
		ling.l_linger = 0;
		Setsockopt(connfd, SOL_SOCKET, SO_LINGER, &ling, sizeof(ling));

        ticks = time(NULL);
        snprintf(buff, sizeof(buff), "%.24s\r\n", ctime(&ticks));
        Write(connfd, buff, strlen(buff));

		sleep(2);	/* let data get across before RST */
		Close(connfd);
	}
}
