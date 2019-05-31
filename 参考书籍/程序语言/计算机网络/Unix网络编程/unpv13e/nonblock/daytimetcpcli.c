#include	"unp.h"

int
main(int argc, char **argv)
{
	int					sockfd, n;
	struct sockaddr_in	servaddr;
	char				recvline[MAXLINE + 1];

	if ( (sockfd = socket(PF_INET, SOCK_STREAM, 0)) < 0)
		err_sys("socket error");

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family      = AF_INET;
	servaddr.sin_addr.s_addr = inet_addr(argv[1]);
	servaddr.sin_port        = htons(13);	/* daytime server */

	if (connect_nonb(sockfd, (SA *) &servaddr, sizeof(servaddr), 0) < 0)
		err_sys("connect error");

	for ( ; ; ) {
		if ( (n = read(sockfd, recvline, MAXLINE)) <= 0) {
			if (n == 0)
				break;		/* server closed connection */
			else
				err_sys("read error");
		}
		recvline[n] = 0;	/* null terminate */
		Fputs(recvline, stdout);
	}
	exit(0);
}
