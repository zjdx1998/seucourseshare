/* include child_make */
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
/* end child_make */

/* include child_main */
void
child_main(int i, int listenfd, int addrlen)
{
	int				connfd;
	void			web_child(int);
	socklen_t		clilen;
	struct sockaddr	*cliaddr;

	cliaddr = Malloc(addrlen);

	printf("child %ld starting\n", (long) getpid());
	for ( ; ; ) {
		clilen = addrlen;
		connfd = Accept(listenfd, cliaddr, &clilen);

		web_child(connfd);		/* process the request */
		Close(connfd);
	}
}
/* end child_main */
