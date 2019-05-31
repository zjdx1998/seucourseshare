#include	"unp.h"
#include	<time.h>

char *
gf_time(void)
{
	struct timeval	tv;
	time_t			t;
	static char		str[30];
	char			*ptr;

	if (gettimeofday(&tv, NULL) < 0)
		err_sys("gettimeofday error");

	t = tv.tv_sec;	/* POSIX says tv.tv_sec is time_t; some BSDs don't agree. */
	ptr = ctime(&t);
	strcpy(str, &ptr[11]);
		/* Fri Sep 13 00:00:00 1986\n\0 */
		/* 0123456789012345678901234 5  */
	snprintf(str+8, sizeof(str)-8, ".%06ld", tv.tv_usec);

	return(str);
}
