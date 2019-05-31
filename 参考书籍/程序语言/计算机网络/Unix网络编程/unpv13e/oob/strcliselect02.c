#include	"unp.h"

void
str_cli(FILE *fp, int sockfd)
{
	int			maxfdp1, stdineof = 0;
	fd_set		rset;
	char		sendline[MAXLINE], recvline[MAXLINE];

	heartbeat_cli(sockfd, 1, 5);

	FD_ZERO(&rset);
	for ( ; ; ) {
		if (stdineof == 0)
			FD_SET(fileno(fp), &rset);
		FD_SET(sockfd, &rset);
		maxfdp1 = max(fileno(fp), sockfd) + 1;
		if (select(maxfdp1, &rset, NULL, NULL, NULL) < 0) {
			if (errno == EINTR)
				continue;
			else
				err_sys("select error");
		}

		if (FD_ISSET(sockfd, &rset)) {	/* socket is readable */
			if (Readline(sockfd, recvline, MAXLINE) == 0) {
				if (stdineof == 1)
					return;		/* normal termination */
				else
					err_quit("str_cli: server terminated prematurely");
			}

			Writen(STDOUT_FILENO, recvline, strlen(recvline));
		}

		if (FD_ISSET(fileno(fp), &rset)) {  /* input is readable */
			if (Fgets(sendline, MAXLINE, fp) == NULL) {
				stdineof = 1;
				alarm(0);			/* turn off heartbeat */
				Shutdown(sockfd, SHUT_WR);	/* send FIN */
				FD_CLR(fileno(fp), &rset);
				continue;
			}

			Writen(sockfd, sendline, strlen(sendline));
		}
	}
}
