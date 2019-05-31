/* test readline() */

#include	"unpthread.h"

static char	*infile;	/* from argv[1]; read-only by threads */

void *
myfunc(void *ptr)
{
	int		i, fdin;
	char	buf[MAXLINE];
	FILE	*fpout;

	snprintf(buf, sizeof(buf), "temp.%d", pthread_self());
	fpout = Fopen(buf, "w+");
	/* printf("created %s\n", buf); */

	for (i = 0; i < 5; i++) {
		fdin = Open(infile, O_RDONLY, 0);

		while (Readline(fdin, buf, sizeof(buf)) > 0) {
			fputs(buf, fpout);
		}
		Close(fdin);
	}
	Fclose(fpout);

	printf("thread %d done\n", pthread_self());
	return(NULL);
}

int
main(int argc, char **argv)
{
	int				i, nthreads;
	pthread_t		tid;

	if (argc != 3)
		err_quit("usage: test04 <input-file> <#threads>");
	infile = argv[1];
	nthreads = atoi(argv[2]);

	for (i = 0; i < nthreads; i++) {
		Pthread_create(&tid, NULL, myfunc, NULL);
	}

	pause();

	exit(0);
}
