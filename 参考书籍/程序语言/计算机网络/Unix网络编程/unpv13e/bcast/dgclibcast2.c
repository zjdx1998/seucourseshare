#include	"unp.h"

static void	recvfrom_alarm(int);

void
dg_cli(FILE *fp, int sockfd, const SA *pservaddr, socklen_t servlen)
{
	int				n;
	const int		on = 1;
	char			sendline[MAXLINE], recvline[MAXLINE + 1];
	socklen_t		len;
	struct sockaddr	*preply_addr;
 
	preply_addr = Malloc(servlen);

	Setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, &on, sizeof(on));

	Signal(SIGALRM, recvfrom_alarm);

	while (Fgets(sendline, MAXLINE, fp) != NULL) {

		Sendto(sockfd, sendline, strlen(sendline), 0, pservaddr, servlen);

		alarm(1);
		for ( ; ; ) {
			len = servlen;
			n = recvfrom(sockfd, recvline, MAXLINE, 0, preply_addr, &len);
			if (n == -1) {
				if (errno == EINTR)
					break;		/* waited long enough for replies */
				else
					err_sys("recvfrom error");
			} else {
				recvline[n] = 0;	/* null terminate */
				sleep(1);
				printf("from %s: %s",
						Sock_ntop_host(preply_addr, len), recvline);
			}
		}
	}
	free(preply_addr);
}

static void
recvfrom_alarm(int signo)
{
	return;		/* just interrupt the recvfrom() */
}
