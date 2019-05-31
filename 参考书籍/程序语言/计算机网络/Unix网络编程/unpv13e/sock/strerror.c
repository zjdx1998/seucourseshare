#include	<stdio.h>

extern const char	*const sys_errlist[];
extern int		sys_nerr;

char *
strerror(int error)
{
	static char	mesg[30];

	if (error >= 0 && error <= sys_nerr)
		return(sys_errlist[error]);

	snprintf(mesg, sizeof(mesg), "Unknown error (%d)", error);
	return(mesg);
}
