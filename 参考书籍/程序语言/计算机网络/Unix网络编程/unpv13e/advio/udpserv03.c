/* include udpserv1 */
#include	"unpifi.h"

void	mydg_echo(int, SA *, socklen_t, SA *);

int
main(int argc, char **argv)
{
	int					sockfd;
	const int			on = 1;
	pid_t				pid;
	struct ifi_info		*ifi, *ifihead;
	struct sockaddr_in	*sa, cliaddr, wildaddr;

	for (ifihead = ifi = Get_ifi_info(AF_INET, 1);
		 ifi != NULL; ifi = ifi->ifi_next) {

			/*4bind unicast address */
		sockfd = Socket(AF_INET, SOCK_DGRAM, 0);

		Setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));

		sa = (struct sockaddr_in *) ifi->ifi_addr;
		sa->sin_family = AF_INET;
		sa->sin_port = htons(SERV_PORT);
		Bind(sockfd, (SA *) sa, sizeof(*sa));
		printf("bound %s\n", Sock_ntop((SA *) sa, sizeof(*sa)));

		if ( (pid = Fork()) == 0) {		/* child */
			mydg_echo(sockfd, (SA *) &cliaddr, sizeof(cliaddr), (SA *) sa);
			exit(0);		/* never executed */
		}
/* end udpserv1 */
/* include udpserv2 */
		if (ifi->ifi_flags & IFF_BROADCAST) {
				/* 4try to bind broadcast address */
			sockfd = Socket(AF_INET, SOCK_DGRAM, 0);
			Setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));

			sa = (struct sockaddr_in *) ifi->ifi_brdaddr;
			sa->sin_family = AF_INET;
			sa->sin_port = htons(SERV_PORT);
			if (bind(sockfd, (SA *) sa, sizeof(*sa)) < 0) {
				if (errno == EADDRINUSE) {
					printf("EADDRINUSE: %s\n",
						   Sock_ntop((SA *) sa, sizeof(*sa)));
					Close(sockfd);
					continue;
				} else
					err_sys("bind error for %s",
							Sock_ntop((SA *) sa, sizeof(*sa)));
			}
			printf("bound %s\n", Sock_ntop((SA *) sa, sizeof(*sa)));

			if ( (pid = Fork()) == 0) {		/* child */
				mydg_echo(sockfd, (SA *) &cliaddr, sizeof(cliaddr),
						  (SA *) sa);
				exit(0);		/* never executed */
			}
		}
	}
/* end udpserv2 */
/* include udpserv3 */
		/* 4bind wildcard address */
	sockfd = Socket(AF_INET, SOCK_DGRAM, 0);
	Setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));

	bzero(&wildaddr, sizeof(wildaddr));
	wildaddr.sin_family = AF_INET;
	wildaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	wildaddr.sin_port = htons(SERV_PORT);
	Bind(sockfd, (SA *) &wildaddr, sizeof(wildaddr));
	printf("bound %s\n", Sock_ntop((SA *) &wildaddr, sizeof(wildaddr)));

	if ( (pid = Fork()) == 0) {		/* child */
		mydg_echo(sockfd, (SA *) &cliaddr, sizeof(cliaddr), (SA *) sa);
		exit(0);		/* never executed */
	}
	exit(0);
}
/* end udpserv3 */

/* include mydg_echo */
void
mydg_echo(int sockfd, SA *pcliaddr, socklen_t clilen, SA *myaddr)
{
	int			n;
	char		mesg[MAXLINE];
	socklen_t	len;

	for ( ; ; ) {
		len = clilen;
		n = Recvfrom(sockfd, mesg, MAXLINE, 0, pcliaddr, &len);
		printf("child %d, datagram from %s", getpid(),
			   Sock_ntop(pcliaddr, len));
		printf(", to %s\n", Sock_ntop(myaddr, clilen));

		Sendto(sockfd, mesg, n, 0, pcliaddr, len);
	}
}
/* end mydg_echo */
