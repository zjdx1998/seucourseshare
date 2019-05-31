#include	"unp.h"

#define	MAXN	16384		/* max #bytes to request from server */

int
main(int argc, char **argv)
{
	int		i, j, fd, nchildren, nloops, nbytes;
	pid_t	pid;
	ssize_t	n;
	char	request[MAXLINE], reply[MAXN];

	if (argc != 6)
		err_quit("usage: client <hostname or IPaddr> <port> <#children> "
				 "<#loops/child> <#bytes/request>");

	nchildren = atoi(argv[3]);
	nloops = atoi(argv[4]);
	nbytes = atoi(argv[5]);
	snprintf(request, sizeof(request), "%d\n", nbytes);	/* newline at end */

	for (i = 0; i < nchildren; i++) {
		if ( (pid = Fork()) == 0) {		/* child */
			for (j = 0; j < nloops; j++) {
				fd = Tcp_connect(argv[1], argv[2]);

				/*
				 * We want to see what happens to the server when it has
				 * connections outstanding and an RST arrives for one of
				 * them, before the connection is accepted.
				 * With the XTI server, this should generate some
				 * T_DISCONNECT events from t_accept(), which must be
				 * handled correctly.
				 *
				 * We do this for every third connection from the third
				 * client child.  (Could add more command-line args ...)
				 */

				if (i == 2 && (j % 3) == 0) {
					struct linger	ling;

					ling.l_onoff = 1;
					ling.l_linger = 0;
					Setsockopt(fd, SOL_SOCKET, SO_LINGER, &ling, sizeof(ling));
					Close(fd);

					/* and just continue on for this client connection ... */
					fd = Tcp_connect(argv[1], argv[2]);
				}

				Write(fd, request, strlen(request));

				if ( (n = Readn(fd, reply, nbytes)) != nbytes)
					err_quit("server returned %d bytes", n);

				Close(fd);		/* TIME_WAIT on client, not server */
			}
			printf("child %d done\n", i);
			exit(0);
		}
		/* parent loops around to fork() again */
	}

	while (wait(NULL) > 0)	/* now parent waits for all children */
		;
	if (errno != ECHILD)
		err_sys("wait error");

	exit(0);
}
