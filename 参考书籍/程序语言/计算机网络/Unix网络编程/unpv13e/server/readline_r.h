typedef struct {
  int		read_fd;		/* caller's descriptor to read from */
  char		*read_ptr;		/* caller's buffer to read into */
  size_t	read_maxlen;	/* max #bytes to read */
				/* next three are used internally by the function */
  int		rl_cnt;			/* initialize to 0 */
  char		*rl_bufptr;		/* initialize to rl_buf */
  char		rl_buf[MAXLINE];
} Rline;

void	readline_rinit(int, void *, size_t, Rline *);
ssize_t	readline_r(Rline *);
ssize_t	Readline_r(Rline *);
