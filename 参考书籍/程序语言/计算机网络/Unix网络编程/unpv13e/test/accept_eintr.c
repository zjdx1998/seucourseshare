#include	"unp.h"

/* Let's see if accept() is automatically restarted by the implementation. */

void
sig_int(int signo)
{
	printf("received SIGINT\n");
	return;
}

int
main(int argc, char **argv)
{
	int					listenfd, connfd;
	socklen_t			clilen;
	struct sockaddr_in	cliaddr, servaddr;

	listenfd = Socket(AF_INET, SOCK_STREAM, 0);

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family      = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port        = htons(SERV_PORT);

	Bind(listenfd, (SA *) &servaddr, sizeof(servaddr));

	Listen(listenfd, LISTENQ);

	Signal(SIGINT, sig_int);	/* sets SA_RESTART */

	for ( ; ; ) {
		clilen = sizeof(cliaddr);
		connfd = Accept(listenfd, (SA *) &cliaddr, &clilen);
			/* error from Accept() if not restarted */
	}
}
