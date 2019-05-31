#include	"unp.h"

void
sig_pipe(int signo)
{
	printf("SIGPIPE received\n");
	return;
}

int
main(int argc, char **argv)
{
	int					sockfd;
	struct sockaddr_in	servaddr;

	if (argc != 2)
		err_quit("usage: tcpcli <IPaddress>");

	sockfd = Socket(AF_INET, SOCK_STREAM, 0);

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(13);		/* daytime server */
	Inet_pton(AF_INET, argv[1], &servaddr.sin_addr);

	Signal(SIGPIPE, sig_pipe);

	Connect(sockfd, (SA *) &servaddr, sizeof(servaddr));

	sleep(2);
	Write(sockfd, "hello", 5);
	sleep(2);
	Write(sockfd, "world", 5);

	exit(0);
}
