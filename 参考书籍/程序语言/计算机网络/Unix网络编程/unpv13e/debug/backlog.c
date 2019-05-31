#include	"unp.h"

#define	PORT		9999
#define	ADDR		"127.0.0.1"
#define	MAXBACKLOG	100

			/* globals */
struct sockaddr_in	serv;
pid_t				pid;	/* of child */

int			pipefd[2];
#define	pfd	pipefd[1]	/* parent's end */
#define	cfd	pipefd[0]	/* child's end */

			/* function prototypes */
void	do_parent(void);
void	do_child(void);

int
main(int argc, char **argv)
{
	if (argc != 1)
		err_quit("usage: backlog");

	Socketpair(AF_UNIX, SOCK_STREAM, 0, pipefd);

	bzero(&serv, sizeof(serv));
	serv.sin_family = AF_INET;
	serv.sin_port = htons(PORT);
	Inet_pton(AF_INET, ADDR, &serv.sin_addr);

	if ( (pid = Fork()) == 0)
		do_child();
	else
		do_parent();
		
	exit(0);
}

void
parent_alrm(int signo)
{
	return;		/* just interrupt blocked connect() */
}

void
do_parent(void)
{
	int		backlog, j, k, junk, fd[MAXBACKLOG + 1];

	Close(cfd);
	Signal(SIGALRM, parent_alrm);

	for (backlog = 0; backlog <= 14; backlog++) {
		printf("backlog = %d: ", backlog);
		Write(pfd, &backlog, sizeof(int));	/* tell child value */
		Read(pfd, &junk, sizeof(int));		/* wait for child */

		for (j = 1; j <= MAXBACKLOG; j++) {
			fd[j] = Socket(AF_INET, SOCK_STREAM, 0);
			alarm(2);
			if (connect(fd[j], (SA * ) &serv, sizeof(serv)) < 0) {
				if (errno != EINTR)
					err_sys("connect error, j = %d", j);
				printf("timeout, %d connections completed\n", j-1);
				for (k = 1; k <= j; k++)
					Close(fd[k]);
				break;	/* next value of backlog */
			}
			alarm(0);
		}
		if (j > MAXBACKLOG)
			printf("%d connections?\n", MAXBACKLOG);
	}
	backlog = -1;		/* tell child we're all done */
	Write(pfd, &backlog, sizeof(int));
}

void
do_child(void)
{
	int		listenfd, backlog, junk;
	const int	on = 1;

	Close(pfd);

	Read(cfd, &backlog, sizeof(int));	/* wait for parent */
	while (backlog >= 0) {
		listenfd = Socket(AF_INET, SOCK_STREAM, 0);
		Setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
		Bind(listenfd, (SA *) &serv, sizeof(serv));
		Listen(listenfd, backlog);		/* start the listen */

		Write(cfd, &junk, sizeof(int));	/* tell parent */

		Read(cfd, &backlog, sizeof(int));/* just wait for parent */
		Close(listenfd);	/* closes all queued connections, too */
	}
}
