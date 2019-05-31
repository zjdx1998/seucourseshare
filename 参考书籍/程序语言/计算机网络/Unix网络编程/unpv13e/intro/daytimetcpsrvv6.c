#include	"unp.h"
#include	<time.h>

int
main(int argc, char **argv)
{
	int					listenfd, connfd;
	socklen_t			len;
	char				buff[MAXLINE];
	time_t				ticks;
	struct sockaddr_in6	servaddr, cliaddr;

	listenfd = Socket(AF_INET6, SOCK_STREAM, 0);

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin6_family = AF_INET6;
	servaddr.sin6_addr   = in6addr_any;
	servaddr.sin6_port   = htons(13);	/* daytime server */

	Bind(listenfd, (SA *) &servaddr, sizeof(servaddr));

	Listen(listenfd, LISTENQ);

	for ( ; ; ) {
		len = sizeof(cliaddr);
		connfd = Accept(listenfd, (SA *) &cliaddr, &len);
		printf("connection from %s\n",
			   Sock_ntop((SA *) &cliaddr, len));

        ticks = time(NULL);
        snprintf(buff, sizeof(buff), "%.24s\r\n", ctime(&ticks));
        Write(connfd, buff, strlen(buff));

		Close(connfd);
	}
}
