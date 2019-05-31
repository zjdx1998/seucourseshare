/* See what the implementation returns for TSD keys */

#include	"unpthread.h"

pthread_key_t	my_key;

int
main(int argc, char **argv)
{
	int		*iptr;

	Pthread_key_create(&my_key, NULL);
	printf("first key = %d\n", my_key);

	Pthread_key_create(&my_key, NULL);
	printf("second key = %d\n", my_key);

	Pthread_key_create(&my_key, NULL);
	printf("third key = %d\n", my_key);

	if ( (iptr = pthread_getspecific((pthread_key_t) 0)) == NULL)
		printf("key 0 pointer is NULL\n");
	else
		printf("value in key 0 = %d\n", *iptr);

	errno = 67;
	if ( (iptr = pthread_getspecific((pthread_key_t) 0)) == NULL)
		printf("key 0 pointer is NULL\n");
	else
		printf("value in key 0 = %d\n", *iptr);

	exit(0);
}
