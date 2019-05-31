/* include web1 */
#include	"web.h"

int
main(int argc, char **argv)
{
	int		i, fd, n, maxnconn, flags, error;
	char	buf[MAXLINE];
	fd_set	rs, ws;

	if (argc < 5)
		err_quit("usage: web <#conns> <hostname> <homepage> <file1> ...");
	maxnconn = atoi(argv[1]);

	nfiles = min(argc - 4, MAXFILES);
	for (i = 0; i < nfiles; i++) {
		file[i].f_name = argv[i + 4];
		file[i].f_host = argv[2];
		file[i].f_flags = 0;
	}
	printf("nfiles = %d\n", nfiles);

	home_page(argv[2], argv[3]);

	FD_ZERO(&rset);
	FD_ZERO(&wset);
	maxfd = -1;
	nlefttoread = nlefttoconn = nfiles;
	nconn = 0;
/* end web1 */
/* include web2 */
	while (nlefttoread > 0) {
		while (nconn < maxnconn && nlefttoconn > 0) {
				/* 4find a file to read */
			for (i = 0 ; i < nfiles; i++)
				if (file[i].f_flags == 0)
					break;
			if (i == nfiles)
				err_quit("nlefttoconn = %d but nothing found", nlefttoconn);
			start_connect(&file[i]);
			nconn++;
			nlefttoconn--;
		}

		rs = rset;
		ws = wset;
		n = Select(maxfd+1, &rs, &ws, NULL, NULL);

		for (i = 0; i < nfiles; i++) {
			flags = file[i].f_flags;
			if (flags == 0 || flags & F_DONE)
				continue;
			fd = file[i].f_fd;
			if (flags & F_CONNECTING &&
				(FD_ISSET(fd, &rs) || FD_ISSET(fd, &ws))) {
				n = sizeof(error);
				if (getsockopt(fd, SOL_SOCKET, SO_ERROR, &error, &n) < 0 ||
					error != 0) {
					err_ret("nonblocking connect failed for %s",
							file[i].f_name);
				}
					/* 4connection established */
				printf("connection established for %s\n", file[i].f_name);
				FD_CLR(fd, &wset);		/* no more writeability test */
				write_get_cmd(&file[i]);/* write() the GET command */

			} else if (flags & F_READING && FD_ISSET(fd, &rs)) {
				if ( (n = Read(fd, buf, sizeof(buf))) == 0) {
					printf("end-of-file on %s\n", file[i].f_name);
					Close(fd);
					file[i].f_flags = F_DONE;	/* clears F_READING */
					FD_CLR(fd, &rset);
					nconn--;
					nlefttoread--;
				} else {
					printf("read %d bytes from %s\n", n, file[i].f_name);
				}
			}
		}
	}
	exit(0);
}
/* end web2 */
