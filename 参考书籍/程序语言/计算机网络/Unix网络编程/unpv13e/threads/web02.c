/* Doesn't work right.  Main thread sucks up all the CPU time polling unless
   we call thr_yield(). */
#include	"unpthread.h"
#include	<thread.h>		/* Solaris threads */

#define	MAXFILES	20
#define	SERV		"80"	/* port number or service name */

struct file {
  char	*f_name;			/* filename */
  char	*f_host;			/* hostname or IP address */
  int    f_fd;				/* descriptor */
  int	 f_flags;			/* F_xxx below */
  pthread_t	 f_tid;			/* thread ID */
} file[MAXFILES];
#define	F_CONNECTING	1	/* connect() in progress */
#define	F_READING		2	/* connect() complete; now reading */
#define	F_DONE			4	/* all done */
#define	F_JOINED		8	/* main has pthread_join'ed */

int		nconn, nfiles, nlefttoconn, nlefttoread;
char	get[] = "GET / HTTP/1.0\r\n\r\n";	/* for home page */

int				ndone;		/* number of terminated threads & mutex */
pthread_mutex_t	ndone_mutex = PTHREAD_MUTEX_INITIALIZER;

void	*do_get_read(void *);
void	home_page(const char *, const char *);
void	write_get_cmd(struct file *);

int
main(int argc, char **argv)
{
	int			i, n, maxnconn;
	pthread_t	tid;
	struct file	*fptr;

	if (argc < 5)
		err_quit("usage: web <#conns> <IPaddr> <homepage> file1 ...");
	maxnconn = atoi(argv[1]);

	nfiles = min(argc - 4, MAXFILES);
	for (i = 0; i < nfiles; i++) {
		file[i].f_name = argv[i + 4];
		file[i].f_host = argv[2];
		file[i].f_flags = 0;
	}
	printf("nfiles = %d\n", nfiles);

	home_page(argv[2], argv[3]);

	nlefttoread = nlefttoconn = nfiles;
	nconn = 0;
	while (nlefttoread > 0) {
/* printf("nconn = %d, nlefttoconn = %d\n", nconn, nlefttoconn); */
		while (nconn < maxnconn && nlefttoconn > 0) {
				/* find a file to read */
			for (i = 0 ; i < nfiles; i++)
				if (file[i].f_flags == 0)
					break;
			if (i == nfiles)
				err_quit("nlefttoconn = %d but nothing found", nlefttoconn);

			if ( (n = pthread_create(&tid, NULL, &do_get_read, &file[i])) != 0)
				errno = n, err_sys("pthread_create error");
printf("created thread %d\n", tid);
			file[i].f_tid = tid;
			file[i].f_flags = F_CONNECTING;
			nconn++;
			nlefttoconn--;
		}
thr_yield();

			/* See if one of the threads is done */
		if ( (n = pthread_mutex_lock(&ndone_mutex)) != 0)
			errno = n, err_sys("pthread_mutex_lock error");
		if (ndone > 0) {
			for (i = 0; i < nfiles; i++) {
				if (file[i].f_flags & F_DONE) {
					if ( (n = pthread_join(file[i].f_tid, (void **) &fptr)) != 0)
						errno = n, err_sys("pthread_join error");
		
					if (&file[i] != fptr)
						err_quit("file[i] != fptr");
					fptr->f_flags = F_JOINED;	/* clears F_DONE */
					ndone--;
					nconn--;
					nlefttoread--;
					printf("thread id %d for %s done\n",
							file[i].f_tid, fptr->f_name);
				}
			}
		}
		if ( (n = pthread_mutex_unlock(&ndone_mutex)) != 0)
			errno = n, err_sys("pthread_mutex_unlock error");
	}

	exit(0);
}

void *
do_get_read(void *vptr)
{
	int					fd, n;
	char				line[MAXLINE];
	struct file			*fptr;

	fptr = (struct file *) vptr;

	fd = Tcp_connect(fptr->f_host, SERV);
	fptr->f_fd = fd;
	printf("do_get_read for %s, fd %d, thread %d\n",
			fptr->f_name, fd, fptr->f_tid);

	write_get_cmd(fptr);	/* write() the GET command */

		/* Read server's reply */
	for ( ; ; ) {
		if ( (n = read(fd, line, MAXLINE)) <= 0) {
			if (n == 0)
				break;		/* server closed connection */
			else
				err_sys("read error");
		}
		printf("read %d bytes from %s\n", n, fptr->f_name);
	}
	printf("end-of-file on %s\n", fptr->f_name);
	close(fd);
	fptr->f_flags = F_DONE;		/* clears F_READING */

	if ( (n = pthread_mutex_lock(&ndone_mutex)) != 0)
		errno = n, err_sys("pthread_mutex_lock error");
	ndone++;
	if ( (n = pthread_mutex_unlock(&ndone_mutex)) != 0)
		errno = n, err_sys("pthread_mutex_unlock error");

	return(fptr);		/* terminate thread */
}

void
write_get_cmd(struct file *fptr)
{
	int		n;
	char	line[MAXLINE];

	strcpy(line, "GET ");
	strcat(line, fptr->f_name);
	strcat(line, " HTTP/1.0\r\n\r\n");
	n = strlen(line);
	if (writen(fptr->f_fd, line, n) != n)
		err_sys("writen error");
	printf("wrote %d bytes for %s\n", n, fptr->f_name);

	fptr->f_flags = F_READING;			/* clears F_CONNECTING */
}

void
home_page(const char *host, const char *fname)
{
	int					fd, n;
	char				line[MAXLINE];

	fd = Tcp_connect(host, SERV);

	strcpy(line, "GET ");
	strcat(line, fname);
	strcat(line, " HTTP/1.0\r\n\r\n");
	n = strlen(line);
	if (writen(fd, line, n) != n)
		err_sys("writen error");

	for ( ; ; ) {
		if ( (n = read(fd, line, MAXLINE)) <= 0) {
			if (n == 0)
				break;		/* server closed connection */
			else
				err_sys("read error");
		}
		printf("read %d bytes of home page\n", n);
		/* do whatever with data */
	}
	printf("end-of-file on home page\n");
	close(fd);
}
