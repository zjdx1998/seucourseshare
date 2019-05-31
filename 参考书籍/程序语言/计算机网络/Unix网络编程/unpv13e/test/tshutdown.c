#include	"unp.h"

#define	BUFF	100000
char	buff[BUFF];

int
main(int argc, char **argv)
{
	int		sockfd, nbytes;
	ssize_t	n;

	if (argc != 3)
		err_quit("usage: tshutdown <hostname> <service>");

	sockfd = Tcp_connect(argv[1], argv[2]);
	fprintf(stderr, "connected\n");

	while ( (n = Read(STDIN_FILENO, buff, BUFF)) > 0) {
		Writen(sockfd, buff, n);
	}
	Close(STDIN_FILENO);
	/* Shutdown(sockfd, SHUT_WR); */
	
	nbytes = 0;
	while ( (n = Read(sockfd, buff, BUFF)) > 0) {
		/* fprintf(stderr, "read %d bytes from socket\n", n); */
		Write(STDOUT_FILENO, buff, n);
		nbytes += n;
	}
	fprintf(stderr, "total: %d bytes read from socket\n", nbytes);
	Close(STDOUT_FILENO);

	exit(0);
}
