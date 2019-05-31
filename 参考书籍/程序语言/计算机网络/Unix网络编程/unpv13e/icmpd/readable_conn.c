/* include readable_conn1 */
#include	"icmpd.h"

int
readable_conn(int i)
{
	int				unixfd, recvfd;
	char			c;
	ssize_t			n;
	socklen_t		len;
	struct sockaddr_storage	ss;

	unixfd = client[i].connfd;
	recvfd = -1;
	if ( (n = Read_fd(unixfd, &c, 1, &recvfd)) == 0) {
		err_msg("client %d terminated, recvfd = %d", i, recvfd);
		goto clientdone;	/* client probably terminated */
	}

		/* 4data from client; should be descriptor */
	if (recvfd < 0) {
		err_msg("read_fd did not return descriptor");
		goto clienterr;
	}
/* end readable_conn1 */

/* include readable_conn2 */
	len = sizeof(ss);
	if (getsockname(recvfd, (SA *) &ss, &len) < 0) {
		err_ret("getsockname error");
		goto clienterr;
	}

	client[i].family = ss.ss_family;
	if ( (client[i].lport = sock_get_port((SA *)&ss, len)) == 0) {
		client[i].lport = sock_bind_wild(recvfd, client[i].family);
		if (client[i].lport <= 0) {
			err_ret("error binding ephemeral port");
			goto clienterr;
		}
	}
	Write(unixfd, "1", 1);	/* tell client all OK */
	Close(recvfd);			/* all done with client's UDP socket */
	return(--nready);

clienterr:
	Write(unixfd, "0", 1);	/* tell client error occurred */
clientdone:
	Close(unixfd);
	if (recvfd >= 0)
		Close(recvfd);
	FD_CLR(unixfd, &allset);
	client[i].connfd = -1;
	return(--nready);
}
/* end readable_conn2 */
