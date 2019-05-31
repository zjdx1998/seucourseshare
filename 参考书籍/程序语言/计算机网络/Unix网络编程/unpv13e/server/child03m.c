#include	"unp.h"

pid_t
child_make(int i, int listenfd, int addrlen)
{
	pid_t	pid;
	void	child_main(int, int, int);

	if ( (pid = Fork()) > 0)
		return(pid);		/* parent */

	child_main(i, listenfd, addrlen);	/* never returns */
}

void
child_main(int i, int listenfd, int addrlen)
{
	int				connfd;
	void			web_child(int);
	socklen_t		clilen;
	struct sockaddr	*cliaddr;
	extern long		*cptr;

	cliaddr = Malloc(addrlen);

	printf("child %ld starting\n", (long) getpid());
	for ( ; ; ) {
		clilen = addrlen;
		my_lock_wait();
		connfd = Accept(listenfd, cliaddr, &clilen);
		my_lock_release();
		cptr[i]++;

		web_child(connfd);		/* process the request */
		Close(connfd);
	}
}
