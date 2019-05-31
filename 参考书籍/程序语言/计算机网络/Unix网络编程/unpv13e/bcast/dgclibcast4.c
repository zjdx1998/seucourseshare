#include	"unp.h"

static void	recvfrom_alarm(int);

void
dg_cli(FILE *fp, int sockfd, const SA *pservaddr, socklen_t servlen)
{
	int				n;
	const int		on = 1;
	char			sendline[MAXLINE], recvline[MAXLINE + 1];
	fd_set			rset;
	sigset_t		sigset_alrm, sigset_empty;
	socklen_t		len;
	struct sockaddr	*preply_addr;
 
	preply_addr = Malloc(servlen);

	Setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, &on, sizeof(on));

	FD_ZERO(&rset);

	Sigemptyset(&sigset_empty);
	Sigemptyset(&sigset_alrm);
	Sigaddset(&sigset_alrm, SIGALRM);

	Signal(SIGALRM, recvfrom_alarm);

	while (Fgets(sendline, MAXLINE, fp) != NULL) {
		Sendto(sockfd, sendline, strlen(sendline), 0, pservaddr, servlen);

		Sigprocmask(SIG_BLOCK, &sigset_alrm, NULL);
		alarm(5);
		for ( ; ; ) {
			FD_SET(sockfd, &rset);
			n = pselect(sockfd+1, &rset, NULL, NULL, NULL, &sigset_empty);
			if (n < 0) {
				if (errno == EINTR)
					break;
				else
					err_sys("pselect error");
			} else if (n != 1)
				err_sys("pselect error: returned %d", n);

			len = servlen;
			n = Recvfrom(sockfd, recvline, MAXLINE, 0, preply_addr, &len);
			recvline[n] = 0;	/* null terminate */
			printf("from %s: %s",
					Sock_ntop_host(preply_addr, len), recvline);
		}
	}
	free(preply_addr);
}

static void
recvfrom_alarm(int signo)
{
	return;		/* just interrupt the recvfrom() */
}
