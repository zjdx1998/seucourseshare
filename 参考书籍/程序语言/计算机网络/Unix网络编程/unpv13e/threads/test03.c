/* test pthread_cond_timedwait() */

#include	"unpthread.h"

int				ndone;
pthread_mutex_t	ndone_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t	ndone_cond = PTHREAD_COND_INITIALIZER;

void *
myfunc(void *ptr)
{
	int		val;

	sleep(100);		/* do not set ndone, do not cond_signal() */
	return(NULL);
}

int
main(int argc, char **argv)
{
	pthread_t		tid;
	int				n, val;
	struct timeval	tv;
	struct timespec	ts;

	if ( (n = pthread_create(&tid, NULL, myfunc, &val)) != 0)
		errno = n, err_sys("pthread_create error");

	if (gettimeofday(&tv, NULL) < 0)
		err_sys("gettimeofday error");
	ts.tv_sec  = tv.tv_sec + 5;		/* 5 seconds in future */
	ts.tv_nsec = tv.tv_usec * 1000;

	if ( (n = pthread_mutex_lock(&ndone_mutex)) != 0)
		errno = n, err_sys("pthread_mutex_lock error");
	while (ndone == 0)
		if ( (n = pthread_cond_timedwait(&ndone_cond, &ndone_mutex, &ts)) != 0){
			if (n == ETIME)
				err_quit("timewait timed out");
			errno = n, err_sys("pthread_cond_timedwait error");
		}

	if ( (n = pthread_mutex_unlock(&ndone_mutex)) != 0)
		errno = n, err_sys("pthread_mutex_unlock error");

	exit(0);
}
