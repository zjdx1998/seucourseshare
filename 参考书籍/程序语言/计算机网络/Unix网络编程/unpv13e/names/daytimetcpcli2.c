#include	"unp.h"

int
main(int argc, char **argv)
{
	int					sockfd, n;
	char				recvline[MAXLINE + 1];
	struct sockaddr_in	servaddr;
	struct in_addr		**pptr, *addrs[2];
	struct hostent		*hp;
	struct servent		*sp;

	if (argc != 3)
		err_quit("usage: daytimetcpcli2 <hostname> <service>");

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;

	if (inet_pton(AF_INET, argv[1], &servaddr.sin_addr) == 1) {
		addrs[0] = &servaddr.sin_addr;
		addrs[1] = NULL;
		pptr = &addrs[0];
	} else if ( (hp = gethostbyname(argv[1])) != NULL) {
		pptr = (struct in_addr **) hp->h_addr_list;
	} else
		err_quit("hostname error for %s: %s", argv[1], hstrerror(h_errno));

	if ( (n = atoi(argv[2])) > 0)
		servaddr.sin_port = htons(n);
	else if ( (sp = getservbyname(argv[2], "tcp")) != NULL)
		servaddr.sin_port = sp->s_port;
	else
		err_quit("getservbyname error for %s", argv[2]);

	for ( ; *pptr != NULL; pptr++) {
		sockfd = Socket(AF_INET, SOCK_STREAM, 0);

		memmove(&servaddr.sin_addr, *pptr, sizeof(struct in_addr));
		printf("trying %s\n",
			   Sock_ntop((SA *) &servaddr, sizeof(servaddr)));

		if (connect(sockfd, (SA *) &servaddr, sizeof(servaddr)) == 0)
			break;		/* success */
		err_ret("connect error");
		close(sockfd);
	}
	if (*pptr == NULL)
		err_quit("unable to connect");

	while ( (n = Read(sockfd, recvline, MAXLINE)) > 0) {
		recvline[n] = 0;	/* null terminate */
		Fputs(recvline, stdout);
	}
	exit(0);
}
