/* include serv00 */
#include	"unp.h"

int
main(int argc, char **argv)
{
	int					listenfd, connfd;
	void				sig_int(int), web_child(int);
	socklen_t			clilen, addrlen;
	struct sockaddr		*cliaddr;

	if (argc == 2)
		listenfd = Tcp_listen(NULL, argv[1], &addrlen);
	else if (argc == 3)
		listenfd = Tcp_listen(argv[1], argv[2], &addrlen);
	else
		err_quit("usage: serv00 [ <host> ] <port#>");
	cliaddr = Malloc(addrlen);

	Signal(SIGINT, sig_int);

	for ( ; ; ) {
		clilen = addrlen;
		connfd = Accept(listenfd, cliaddr, &clilen);

		web_child(connfd);		/* process the request */

		Close(connfd);			/* parent closes connected socket */
	}
}
/* end serv00 */

/* include sigint */
void
sig_int(int signo)
{
	void	pr_cpu_time(void);

	pr_cpu_time();
	exit(0);
}
/* end sigint */
