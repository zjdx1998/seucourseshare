#include	"unp.h"

void
sig_alrm(int signo)
{
}

int
main(int argc, char **argv)
{
	int					sockfd, n;
	struct sockaddr_in	servaddr;
	struct itimerval	val;
	fd_set				rset, wset;

	if (argc != 2)
		err_quit("usage: a.out <IPaddress>");

	if ( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		err_sys("socket error");

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port   = htons(13);	/* echo server */
	Inet_pton(AF_INET, argv[1], &servaddr.sin_addr);

		/* Set interval timer to go off before 3WHS completes */
	Signal(SIGALRM, sig_alrm);
	val.it_interval.tv_sec  = 0;
	val.it_interval.tv_usec = 0;
	val.it_value.tv_sec  = 0;
	val.it_value.tv_usec = 50000;	/* 50 ms */
	if (setitimer(ITIMER_REAL, &val, NULL) == -1)
		err_sys("setitimer error");

again:
	if (connect(sockfd, (SA *) &servaddr, sizeof(servaddr)) < 0) {
		if (errno == EINTR) {
#ifdef	notdef
			goto again;	/* second call to connect() -> EADDRINUSE */
#endif
#ifdef	notdef
			printf("interrupted system call\n");
			exit(0);
#endif
		} else
			err_sys("connect error");
	}

	FD_ZERO(&rset);
	FD_SET(sockfd, &rset);
	wset = rset;
sleep(4);
	n = Select(sockfd+1, &rset, &wset, NULL, NULL);
	printf("select returned %d\n", n);
	if (FD_ISSET(sockfd, &rset))
		printf("socket is readable\n");
	if (FD_ISSET(sockfd, &wset))
		printf("socket is writable\n");

	str_cli(stdin, sockfd);		/* do it all */

	exit(0);
}
