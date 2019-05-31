typedef struct {
  pthread_t		thread_tid;		/* thread ID */
  long			thread_count;	/* # connections handled */
} Thread;
Thread	*tptr;		/* array of Thread structures; calloc'ed */

#define	MAXNCLI	32
int					clifd[MAXNCLI], iget, iput;
pthread_mutex_t		clifd_mutex;
pthread_cond_t		clifd_cond;
