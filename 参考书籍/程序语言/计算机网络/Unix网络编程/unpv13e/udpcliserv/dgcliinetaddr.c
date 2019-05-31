#include	"unp.h"

void
dg_cli(FILE *fp, int sockfd, const SA *pservaddr, socklen_t servlen)
{
	int				n;
	char			sendline[MAXLINE], recvline[MAXLINE + 1];
	socklen_t		len;
	struct sockaddr_in	*replyaddr;

	replyaddr = Malloc(servlen);

	while (Fgets(sendline, MAXLINE, fp) != NULL) {

		Sendto(sockfd, sendline, strlen(sendline), 0, pservaddr, servlen);

		len = servlen;
		n = Recvfrom(sockfd, recvline, MAXLINE, 0, (SA *) replyaddr, &len);
		printf("received reply from %s, port %d\n",
			   inet_ntoa(replyaddr->sin_addr), htons(replyaddr->sin_port));

		recvline[n] = 0;	/* null terminate */
		Fputs(recvline, stdout);
	}
}
