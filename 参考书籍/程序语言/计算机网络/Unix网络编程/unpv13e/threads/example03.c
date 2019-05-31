#include	"unpthread.h"

#define	Pthread_mutex_lock(mptr) \
	{	int  n; \
		if ( (n = pthread_mutex_lock(mptr)) != 0) \
			{ errno = n; err_sys("pthread_mutex_lock error"); } \
	}
#define	Pthread_mutex_unlock(mptr) \
	{	int  n; \
		if ( (n = pthread_mutex_unlock(mptr)) != 0) \
			{ errno = n; err_sys("pthread_mutex_unlock error"); } \
	}
#define	Pthread_cond_wait(cptr,mptr) \
	{	int  n; \
		if ( (n = pthread_cond_wait(cptr,mptr)) != 0) \
			{ errno = n; err_sys("pthread_cond_wait error"); } \
	}
#define	Pthread_cond_signal(cptr) \
	{	int  n; \
		if ( (n = pthread_cond_signal(cptr)) != 0) \
			{ errno = n; err_sys("pthread_cond_signal error"); } \
	}

#define	NLOOP	   	 50
#define	BUFFSIZE	 10

struct buf_t {
  int		b_buf[BUFFSIZE];	/* the buffer which contains integer items */
  int		b_nitems;			/* #items currently in buffer */
  int		b_nextget;
  int		b_nextput;
  pthread_mutex_t	b_mutex;
  pthread_cond_t	b_cond_consumer;	/* consumer waiting to get */
  pthread_cond_t	b_cond_producer;	/* producer waiting to put */
} buf_t;

void	*produce_loop(void *);
void	*consume_loop(void *);

int
main(int argc, char **argv)
{
	int			n;
	pthread_t	tidA, tidB;

	printf("main, addr(stack) = %x, addr(global) = %x, addr(func) = %x\n",
			&n, &buf_t, &produce_loop);
	if ( (n = pthread_create(&tidA, NULL, &produce_loop, NULL)) != 0)
		errno = n, err_sys("pthread_create error for A");
	if ( (n = pthread_create(&tidB, NULL, &consume_loop, NULL)) != 0)
		errno = n, err_sys("pthread_create error for B");

		/* wait for both threads to terminate */
	if ( (n = pthread_join(tidA, NULL)) != 0)
		errno = n, err_sys("pthread_join error for A");
	if ( (n = pthread_join(tidB, NULL)) != 0)
		errno = n, err_sys("pthread_join error for B");

	exit(0);
}

void
produce(struct buf_t *bptr, int val)
{
	Pthread_mutex_lock(&bptr->b_mutex);
		/* Wait if buffer is full */
	while (bptr->b_nitems >= BUFFSIZE)
		Pthread_cond_wait(&bptr->b_cond_producer, &bptr->b_mutex);

		/* There is room, store the new value */
	printf("produce %d\n", val);
	bptr->b_buf[bptr->b_nextput] = val;
	if (++bptr->b_nextput >= BUFFSIZE)
		bptr->b_nextput = 0;
	bptr->b_nitems++;

		/* Signal consumer */
	Pthread_cond_signal(&bptr->b_cond_consumer);
	Pthread_mutex_unlock(&bptr->b_mutex);
}

int
consume(struct buf_t *bptr)
{
	int		val;

	Pthread_mutex_lock(&bptr->b_mutex);
		/* Wait if buffer is empty */
	while (bptr->b_nitems <= 0)
		Pthread_cond_wait(&bptr->b_cond_consumer, &bptr->b_mutex);

		/* There is data, fetch the value */
	val = bptr->b_buf[bptr->b_nextget];
	printf("consume %d\n", val);
	if (++bptr->b_nextget >= BUFFSIZE)
		bptr->b_nextget = 0;
	bptr->b_nitems--;

		/* Signal producer; it might be waiting for space to store */
	Pthread_cond_signal(&bptr->b_cond_producer);
	Pthread_mutex_unlock(&bptr->b_mutex);

	return(val);
}

void *
produce_loop(void *vptr)
{
	int		i;

	printf("produce_loop thread, addr(stack) = %x\n", &i);
	for (i = 0; i < NLOOP; i++) {
		produce(&buf_t, i);
	}

	return(NULL);
}

void *
consume_loop(void *vptr)
{
	int		i, val;

	printf("consume_loop thread, addr(stack) = %x\n", &i);
	for (i = 0; i < NLOOP; i++) {
		val = consume(&buf_t);
	}

	return(NULL);
}
