#include	"unpthread.h"

void	*copyto(void *);

static int	sockfd;
static FILE	*fp;
static int	done;

void
str_cli(FILE *fp_arg, int sockfd_arg)
{
	char		recvline[MAXLINE];
	pthread_t	tid;

	sockfd = sockfd_arg;	/* copy arguments to externals */
	fp = fp_arg;

	Pthread_create(&tid, NULL, copyto, NULL);

	while (Readline(sockfd, recvline, MAXLINE) > 0)
		Fputs(recvline, stdout);

	if (done == 0)
		err_quit("server terminated prematurely");
}

void *
copyto(void *arg)
{
	char	sendline[MAXLINE];

	while (Fgets(sendline, MAXLINE, fp) != NULL)
		Writen(sockfd, sendline, strlen(sendline));

	Shutdown(sockfd, SHUT_WR);	/* EOF on stdin, send FIN */

	done = 1;
	return(NULL);
	/* return (i.e., thread terminates) when end-of-file on stdin */
}
