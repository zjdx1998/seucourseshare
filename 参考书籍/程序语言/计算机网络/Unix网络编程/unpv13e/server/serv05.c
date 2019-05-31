/* include serv05a */
#include	"unp.h"
#include	"child.h"

static int		nchildren;

int
main(int argc, char **argv)
{
	int			listenfd, i, navail, maxfd, nsel, connfd, rc;
	void		sig_int(int);
	pid_t		child_make(int, int, int);
	ssize_t		n;
	fd_set		rset, masterset;
	socklen_t	addrlen, clilen;
	struct sockaddr	*cliaddr;

	if (argc == 3)
		listenfd = Tcp_listen(NULL, argv[1], &addrlen);
	else if (argc == 4)
		listenfd = Tcp_listen(argv[1], argv[2], &addrlen);
	else
		err_quit("usage: serv05 [ <host> ] <port#> <#children>");

	FD_ZERO(&masterset);
	FD_SET(listenfd, &masterset);
	maxfd = listenfd;
	cliaddr = Malloc(addrlen);

	nchildren = atoi(argv[argc-1]);
	navail = nchildren;
	cptr = Calloc(nchildren, sizeof(Child));

		/* 4prefork all the children */
	for (i = 0; i < nchildren; i++) {
		child_make(i, listenfd, addrlen);	/* parent returns */
		FD_SET(cptr[i].child_pipefd, &masterset);
		maxfd = max(maxfd, cptr[i].child_pipefd);
	}

	Signal(SIGINT, sig_int);

	for ( ; ; ) {
		rset = masterset;
		if (navail <= 0)
			FD_CLR(listenfd, &rset);	/* turn off if no available children */
		nsel = Select(maxfd + 1, &rset, NULL, NULL, NULL);

			/* 4check for new connections */
		if (FD_ISSET(listenfd, &rset)) {
			clilen = addrlen;
			connfd = Accept(listenfd, cliaddr, &clilen);

			for (i = 0; i < nchildren; i++)
				if (cptr[i].child_status == 0)
					break;				/* available */

			if (i == nchildren)
				err_quit("no available children");
			cptr[i].child_status = 1;	/* mark child as busy */
			cptr[i].child_count++;
			navail--;

			n = Write_fd(cptr[i].child_pipefd, "", 1, connfd);
			Close(connfd);
			if (--nsel == 0)
				continue;	/* all done with select() results */
		}

			/* 4find any newly-available children */
		for (i = 0; i < nchildren; i++) {
			if (FD_ISSET(cptr[i].child_pipefd, &rset)) {
				if ( (n = Read(cptr[i].child_pipefd, &rc, 1)) == 0)
					err_quit("child %d terminated unexpectedly", i);
				cptr[i].child_status = 0;
				navail++;
				if (--nsel == 0)
					break;	/* all done with select() results */
			}
		}
	}
}
/* end serv05a */

void
sig_int(int signo)
{
	int		i;
	void	pr_cpu_time(void);

		/* 4terminate all children */
	for (i = 0; i < nchildren; i++)
		kill(cptr[i].child_pid, SIGTERM);
	while (wait(NULL) > 0)		/* wait for all children */
		;
	if (errno != ECHILD)
		err_sys("wait error");

	pr_cpu_time();

	for (i = 0; i < nchildren; i++)
		printf("child %d, %ld connections\n", i, cptr[i].child_count);

	exit(0);
}
