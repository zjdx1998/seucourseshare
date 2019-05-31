/* include signal_intr */
#include	"unp.h"

Sigfunc *
signal_intr(int signo, Sigfunc *func)
{
	struct sigaction	act, oact;

	act.sa_handler = func;
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;
#ifdef	SA_INTERRUPT	/* SunOS */
	act.sa_flags |= SA_INTERRUPT;
#endif
	if (sigaction(signo, &act, &oact) < 0)
		return(SIG_ERR);
	return(oact.sa_handler);
}
/* end signal_intr */

Sigfunc *
Signal_intr(int signo, Sigfunc *func)
{
	Sigfunc	*sigfunc;

	if ( (sigfunc = signal_intr(signo, func)) == SIG_ERR)
		err_sys("signal_intr error");
	return(sigfunc);
}
