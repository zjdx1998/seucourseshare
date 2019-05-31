#include	"unp.h"

void
str_cli(FILE *fp, int sockfd)
{
	int				kq, i, n, nev, stdineof = 0, isfile;
	char			buf[MAXLINE];
	struct kevent	kev[2];
	struct timespec	ts;
	struct stat		st;

	isfile = ((fstat(fileno(fp), &st) == 0) &&
			  (st.st_mode & S_IFMT) == S_IFREG);

	EV_SET(&kev[0], fileno(fp), EVFILT_READ, EV_ADD, 0, 0, NULL);
	EV_SET(&kev[1], sockfd, EVFILT_READ, EV_ADD, 0, 0, NULL);

	kq = Kqueue();
	ts.tv_sec = ts.tv_nsec = 0;
	Kevent(kq, kev, 2, NULL, 0, &ts);

	for ( ; ; ) {
		nev = Kevent(kq, NULL, 0, kev, 2, NULL);

		for (i = 0; i < nev; i++) {
			if (kev[i].ident == sockfd) {	/* socket is readable */
				if ( (n = Read(sockfd, buf, MAXLINE)) == 0) {
					if (stdineof == 1)
						return;		/* normal termination */
					else
						err_quit("str_cli: server terminated prematurely");
				}

				Write(fileno(stdout), buf, n);
			}

			if (kev[i].ident == fileno(fp)) {  /* input is readable */
				n = Read(fileno(fp), buf, MAXLINE);
				if (n > 0)
					Writen(sockfd, buf, n);

				if (n == 0 || (isfile && n == kev[i].data)) {
					stdineof = 1;
					Shutdown(sockfd, SHUT_WR);	/* send FIN */
					kev[i].flags = EV_DELETE;
					Kevent(kq, &kev[i], 1, NULL, 0, &ts);	/* remove kevent */
					continue;
				}
			}
		}
	}
}
