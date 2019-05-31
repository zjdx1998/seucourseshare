#include	"unp.h"
#include	<syslog.h>

#define	MAXFD	64

extern int	daemon_proc;	/* defined in error.c */

int
daemon_init(const char *pname, int facility)
{
	int		i;
	pid_t	pid;

	if ( (pid = Fork()) < 0)
		return (-1);
	else if (pid)
		_exit(0);			/* parent terminates */

	/* child 1 continues... */

	if (setsid() < 0)			/* become session leader */
		return (-1);

	Signal(SIGHUP, SIG_IGN);
	if ( (pid = Fork()) < 0)
		return (-1);
	else if (pid)
		_exit(0);			/* child 1 terminates */

	/* child 2 continues... */

	daemon_proc = 1;			/* for err_XXX() functions */

	chdir("/");				/* change working directory */

	/* close off file descriptors */
	for (i = 0; i < MAXFD; i++)
		close(i);

	/* redirect stdin, stdout, and stderr to /dev/null */
	open("/dev/null", O_RDONLY);
	open("/dev/null", O_RDWR);
	open("/dev/null", O_RDWR);

	openlog(pname, LOG_PID, facility);

	return (0);				/* success */
}
