/* include readline */
#include	"unp.h"
#include	"readline_r.h"

static ssize_t
my_read_r(Rline *rptr, char *ptr)
{
	if (rptr->rl_cnt <= 0) {
again:
		rptr->rl_cnt = read(rptr->read_fd, rptr->rl_buf, sizeof(rptr->rl_buf));
		if (rptr->rl_cnt < 0) {
			if (errno == EINTR)
				goto again;
			else
				return(-1);
		}
		else if (rptr->rl_cnt == 0)
			return(0);
		rptr->rl_bufptr = rptr->rl_buf;
	}

	rptr->rl_cnt--;
	*ptr = *rptr->rl_bufptr++ & 255;
	return(1);
}

void
readline_rinit(int fd, void *ptr, size_t maxlen, Rline *rptr)
{
	rptr->read_fd = fd;				/* save caller's arguments */
	rptr->read_ptr = ptr;
	rptr->read_maxlen = maxlen;

	rptr->rl_cnt = 0;				/* and init our counter & pointer */
	rptr->rl_bufptr = rptr->rl_buf;
}

ssize_t
readline_r(Rline *rptr)
{
	int		n, rc;
	char	c, *ptr;

	ptr = rptr->read_ptr;
	for (n = 1; n < rptr->read_maxlen; n++) {
		if ( (rc = my_read_r(rptr, &c)) == 1) {
			*ptr++ = c;
			if (c == '\n')
				break;
		} else if (rc == 0) {
			if (n == 1)
				return(0);	/* EOF, no data read */
			else
				break;		/* EOF, some data was read */
		} else
			return(-1);	/* error */
	}

	*ptr = 0;
	return(n);
}
/* end readline */

ssize_t
Readline_r(Rline *rptr)
{
	ssize_t		n;

	if ( (n = readline_r(rptr)) == -1)
		err_sys("readline_r error");
	return(n);
}
