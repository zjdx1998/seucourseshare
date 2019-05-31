#include	"trace.h"

int gotalarm;

void
sig_alrm(int signo)
{
	gotalarm = 1;	/* set flag to note that alarm occurred */
	return;			/* and interrupt the recvfrom() */
}
