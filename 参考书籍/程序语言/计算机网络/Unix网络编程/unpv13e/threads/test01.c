#include	"unpthread.h"

void *
myfunc(void *ptr)
{
	pause();
}

int
main(int argc, char **argv)
{
	pthread_t	tid;
	int			n;

	/* Let's see what the return value is and what errno is after a error. */
	for ( ; ; ) {
		errno = 0;
		if ( (n = pthread_create(&tid, NULL, myfunc, NULL)) != 0) {
			printf("pthread_create returned %d, errno = %d\n", n, errno);

			errno = 0;
			n = pthread_join(777777, NULL);
			printf("pthread_join returned %d, errno = %d\n", n, errno);

			exit(0);
		}
		printf("created tid %d\n", tid);
	}
}
