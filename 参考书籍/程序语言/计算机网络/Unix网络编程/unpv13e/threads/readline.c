/* include readline1 */
#include	"unpthread.h"

static pthread_key_t	rl_key;
static pthread_once_t	rl_once = PTHREAD_ONCE_INIT;

static void
readline_destructor(void *ptr)
{
	free(ptr);
}

static void
readline_once(void)
{
	Pthread_key_create(&rl_key, readline_destructor);
}

typedef struct {
  int	 rl_cnt;			/* initialize to 0 */
  char	*rl_bufptr;			/* initialize to rl_buf */
  char	 rl_buf[MAXLINE];
} Rline;
/* end readline1 */

/* include readline2 */
static ssize_t
my_read(Rline *tsd, int fd, char *ptr)
{
	if (tsd->rl_cnt <= 0) {
again:
		if ( (tsd->rl_cnt = read(fd, tsd->rl_buf, MAXLINE)) < 0) {
			if (errno == EINTR)
				goto again;
			return(-1);
		} else if (tsd->rl_cnt == 0)
			return(0);
		tsd->rl_bufptr = tsd->rl_buf;
	}

	tsd->rl_cnt--;
	*ptr = *tsd->rl_bufptr++;
	return(1);
}

ssize_t
readline(int fd, void *vptr, size_t maxlen)
{
	size_t		n, rc;
	char	c, *ptr;
	Rline	*tsd;

	Pthread_once(&rl_once, readline_once);
	if ( (tsd = pthread_getspecific(rl_key)) == NULL) {
		tsd = Calloc(1, sizeof(Rline));		/* init to 0 */
		Pthread_setspecific(rl_key, tsd);
	}

	ptr = vptr;
	for (n = 1; n < maxlen; n++) {
		if ( (rc = my_read(tsd, fd, &c)) == 1) {
			*ptr++ = c;
			if (c == '\n')
				break;
		} else if (rc == 0) {
			*ptr = 0;
			return(n - 1);		/* EOF, n - 1 bytes read */
		} else
			return(-1);		/* error, errno set by read() */
	}

	*ptr = 0;
	return(n);
}
/* end readline2 */

ssize_t
Readline(int fd, void *ptr, size_t maxlen)
{
	ssize_t		n;

	if ( (n = readline(fd, ptr, maxlen)) < 0)
		err_sys("readline error");
	return(n);
}
