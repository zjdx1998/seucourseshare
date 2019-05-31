#include	"unp.h"
#include	<setjmp.h>

static void			recvfrom_alarm(int);
static sigjmp_buf	jmpbuf;

void
dg_cli(FILE *fp, int sockfd, const SA *pservaddr, socklen_t servlen)
{
	int				n;
	char			sendline[MAXLINE], recvline[MAXLINE + 1];
	socklen_t		len;
	struct sockaddr	*preply_addr;
 
	preply_addr = Malloc(servlen);

	Signal(SIGALRM, recvfrom_alarm);

	while (Fgets(sendline, MAXLINE, fp) != NULL) {

		Sendto(sockfd, sendline, strlen(sendline), 0, pservaddr, servlen);

		alarm(5);
		for ( ; ; ) {
			if (sigsetjmp(jmpbuf, 1) != 0)
				break;
			len = servlen;
			n = Recvfrom(sockfd, recvline, MAXLINE, 0, preply_addr, &len);
			recvline[n] = 0;	/* null terminate */
			printf("from %s: %s",
					Sock_ntop_host(preply_addr, servlen), recvline);
		}
	}
}

static void
recvfrom_alarm(int signo)
{
	siglongjmp(jmpbuf, 1);
}
