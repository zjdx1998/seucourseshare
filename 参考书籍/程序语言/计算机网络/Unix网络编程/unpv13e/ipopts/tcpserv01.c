#include	"unp.h"

int
main(int argc, char **argv)
{
	int					listenfd, connfd;
	u_char				*opts;
	pid_t				childpid;
	socklen_t			clilen, len;
	struct sockaddr_in	cliaddr, servaddr;
	void				sig_chld(int);

	opts = Malloc(44);

	listenfd = Socket(AF_INET, SOCK_STREAM, 0);

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family      = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port        = htons(SERV_PORT);

	Bind(listenfd, (SA *) &servaddr, sizeof(servaddr));

	Listen(listenfd, LISTENQ);

	Signal(SIGCHLD, sig_chld);

	for ( ; ; ) {
		clilen = sizeof(cliaddr);
		if ( (connfd = accept(listenfd, (SA *) &cliaddr, &clilen)) < 0) {
			if (errno == EINTR)
				continue;		/* back to for() */
			else
				err_sys("accept error");
		}

		len = 44;
		Getsockopt(connfd, IPPROTO_IP, IP_OPTIONS, opts, &len);
		if (len > 0) {
			printf("received IP options, len = %d\n", len);
			inet_srcrt_print(opts, len);
		}

		if ( (childpid = Fork()) == 0) {	/* child process */
			Close(listenfd);	/* close listening socket */
			str_echo(connfd);	/* process the request */
			exit(0);
		}
		Close(connfd);			/* parent closes connected socket */
	}
}
