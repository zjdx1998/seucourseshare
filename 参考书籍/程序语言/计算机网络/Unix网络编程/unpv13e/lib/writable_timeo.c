/* include writable_timeo */
#include	"unp.h"

int
writable_timeo(int fd, int sec)
{
	fd_set			wset;
	struct timeval	tv;

	FD_ZERO(&wset);
	FD_SET(fd, &wset);

	tv.tv_sec = sec;
	tv.tv_usec = 0;

	return(select(fd+1, NULL, &wset, NULL, &tv));
		/* > 0 if descriptor is writable */
}
/* end writable_timeo */

int
Writable_timeo(int fd, int sec)
{
	int		n;

	if ( (n = writable_timeo(fd, sec)) < 0)
		err_sys("writable_timeo error");
	return(n);
}
