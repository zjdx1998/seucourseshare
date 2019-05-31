#include	"unpxti.h"

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
		err_quit("usage: qlen");

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

/* include qlen */
void
do_parent(void)
{
	int				qlen, j, k, junk, fd[MAXBACKLOG + 1];
	struct t_call	tcall;

	Close(cfd);
	Signal(SIGALRM, parent_alrm);

	for (qlen = 0; qlen <= 14; qlen++) {
		printf("qlen = %d: ", qlen);
		Write(pfd, &qlen, sizeof(int));	/* tell child value */
		Read(pfd, &junk, sizeof(int));	/* wait for child */

		for (j = 0; j <= MAXBACKLOG; j++) {
			fd[j] = T_open(XTI_TCP, O_RDWR, NULL);
			T_bind(fd[j], NULL, NULL);

			tcall.addr.maxlen = sizeof(serv);
			tcall.addr.len = sizeof(serv);
			tcall.addr.buf = &serv;
			tcall.opt.len = 0;
			tcall.udata.len = 0;

			alarm(2);
			if (t_connect(fd[j], &tcall, NULL) < 0) {
				if (errno != EINTR)
					err_xti("t_connect error, j = %d", j);
				printf("timeout, %d connections completed\n", j-1);
				for (k = 1; k < j; k++)
					T_close(fd[k]);
				break;	/* next value of qlen */
			}
			alarm(0);
		}
		if (j > MAXBACKLOG)
			printf("%d connections?\n", MAXBACKLOG);
	}
	qlen = -1;		/* tell child we're all done */
	Write(pfd, &qlen, sizeof(int));
}

void
do_child(void)
{
	int				listenfd, qlen, junk;
	struct t_bind	tbind, tbindret;

	Close(pipefd[1]);

	Read(cfd, &qlen, sizeof(int));	/* wait for parent */
	while (qlen >= 0) {
		listenfd = T_open(XTI_TCP, O_RDWR, NULL);

		tbind.addr.maxlen = sizeof(serv);
		tbind.addr.len = sizeof(serv);
		tbind.addr.buf = &serv;
		tbind.qlen = qlen;

		tbindret.addr.maxlen = 0;
		tbindret.addr.len = 0;

		T_bind(listenfd, &tbind, &tbindret);
		printf("returned qlen = %d, ", tbindret.qlen);
		fflush(stdout);

		Write(cfd, &junk, sizeof(int));	/* tell parent */

		Read(cfd, &qlen, sizeof(int));	/* just wait for parent */
		T_close(listenfd);	/* closes all queued connections too */
	}
}
/* end qlen */
