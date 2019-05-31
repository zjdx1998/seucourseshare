#include	"unp.h"

/* Try and get ENOBUFS from sendto() by sending huge datagrams.
   But I still cannot get the error. */

#define	NDG		 2000	/* datagrams to send */
#define	DGLEN	65507	/* length of each datagram */

void
dg_cli(FILE *fp, int sockfd, const SA *pservaddr, socklen_t servlen)
{
	int		i, n;
	char	sendline[DGLEN];

	n = 100 * 1024;
	Setsockopt(sockfd, SOL_SOCKET, SO_SNDBUF, &n, sizeof(n));

	for (i = 0; i < NDG; i++) {
		Sendto(sockfd, sendline, DGLEN, 0, pservaddr, servlen);
	}
}
