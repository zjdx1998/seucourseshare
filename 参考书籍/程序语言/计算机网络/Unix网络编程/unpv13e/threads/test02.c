#include	"unpthread.h"

void *
myfunc(void *ptr)
{
	int		val;

	printf("thread ID of myfunc: %d\n", pthread_self());

	val = *((int *) ptr);
	printf("val = %d\n", val);
	sleep(10);
	val = *((int *) ptr);
	printf("val = %d\n", val);
}

int
main(int argc, char **argv)
{
	pthread_t	tid;
	int			n, val;

	printf("thread ID of main: %d\n", pthread_self());

	/* Let's verify that the value pointed to the thread's argument is
	   modifiable */
	val = 123;
	if ( (n = pthread_create(&tid, NULL, myfunc, &val)) != 0)
		errno = n, err_sys("pthread_create error");
	sleep(5);
	val = 789;
	sleep(20);
	exit(0);
}
