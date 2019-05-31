/* include readline */
#include	"unp.h"

static ssize_t
my_read(int fd, char *ptr)
{
	static int	read_cnt = 0;
	static char	*read_ptr;
	static char	read_buf[MAXLINE];

	if (read_cnt <= 0) {
		if ( (read_cnt = read(fd, read_buf, sizeof(read_buf))) < 0)
			err_sys("read error");
		else if (read_cnt == 0)
			return(0);
		read_ptr = read_buf;
	}

	read_cnt--;
	*ptr = *read_ptr++ & 255;
	return(1);
}

ssize_t
readline(int fd, void *vptr, size_t maxlen)
{
	int		n, rc;
	char	c, *ptr;

	ptr = vptr;
	for (n = 1; n < maxlen; n++) {
		if ( (rc = my_read(fd, &c)) == 1) {
			*ptr++ = c;
			if (c == '\n')
				break;
		} else if (rc == 0) {
			*ptr = 0;
			return(n - 1);	/* EOF, n - 1 bytes were read */
		} else
			return(-1);	/* error */
	}

	*ptr = 0;
	return(n);
}
/* end readline */

ssize_t
Readline(int fd, void *ptr, size_t maxlen)
{
	ssize_t		n;

	if ( (n = readline(fd, ptr, maxlen)) == -1)
		err_sys("readline error");
	return(n);
}
