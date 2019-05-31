#include	"unp.h"

int
main(int argc, char **argv)
{
	int					sockfd;
	void				sig_alrm(int);
	struct itimerval	val;
	struct sockaddr_in	servaddr;

	if (argc != 2)
		err_quit("usage: tcpcli <IPaddress>");

	sockfd = Socket(AF_INET, SOCK_STREAM, 0);

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(SERV_PORT);
	Inet_pton(AF_INET, argv[1], &servaddr.sin_addr);

		/* Set interval timer to go off before 3WHS completes */
	Signal(SIGALRM, sig_alrm);
	val.it_interval.tv_sec  = 0;
	val.it_interval.tv_usec = 0;
	val.it_value.tv_sec  = 0;
	val.it_value.tv_usec = 50000;	/* 50 ms */
	if (setitimer(ITIMER_REAL, &val, NULL) == -1)
		err_sys("setitimer error");

	Connect(sockfd, (SA *) &servaddr, sizeof(servaddr));

	str_cli(stdin, sockfd);		/* do it all */

	exit(0);
}

void
sig_alrm(int signo)
{
	exit(0);
}
