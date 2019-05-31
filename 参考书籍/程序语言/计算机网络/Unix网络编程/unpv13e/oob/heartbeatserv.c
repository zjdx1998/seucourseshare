#include	"unp.h"

static int	servfd;
static int	nsec;			/* #seconds between each alarm */
static int	maxnalarms;		/* #alarms w/no client probe before quit */
static int	nprobes;		/* #alarms since last client probe */
static void	sig_urg(int), sig_alrm(int);

void
heartbeat_serv(int servfd_arg, int nsec_arg, int maxnalarms_arg)
{
	servfd = servfd_arg;		/* set globals for signal handlers */
	if ( (nsec = nsec_arg) < 1)
		nsec = 1;
	if ( (maxnalarms = maxnalarms_arg) < nsec)
		maxnalarms = nsec;

	Signal(SIGURG, sig_urg);
	Fcntl(servfd, F_SETOWN, getpid());

	Signal(SIGALRM, sig_alrm);
	alarm(nsec);
}

static void
sig_urg(int signo)
{
	int		n;
	char	c;
 
	if ( (n = recv(servfd, &c, 1, MSG_OOB)) < 0) {
		if (errno != EWOULDBLOCK)
			err_sys("recv error");
	}
	Send(servfd, &c, 1, MSG_OOB);	/* echo back out-of-band byte */

	nprobes = 0;			/* reset counter */
	return;					/* may interrupt server code */
}

static void
sig_alrm(int signo)
{
	if (++nprobes > maxnalarms) {
		printf("no probes from client\n");
		exit(0);
	}
	alarm(nsec);
	return;					/* may interrupt server code */
}
