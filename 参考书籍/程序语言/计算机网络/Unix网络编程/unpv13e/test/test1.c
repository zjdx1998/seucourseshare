#include	"test.h"

/*
 * Socket test program.
 * Try and figure out everything that we can automatically.
 * Lines preceded by a + are deviations from 4.4BSD.
 * Lines preceded by a ! are fatal errors.
 * Lines without a + or ! are just informational.
 */

	/* allocate globals */
struct sockaddr_in	servaddr, cliaddr;
char	buff[8192];
int		verbose;

/*
 * Check whether various header flags are defined.
 */

void
header_flags()
{

			/* these are all "if not defined" */
#ifndef	MSG_DONTROUTE
	printf("+ MSG_DONTROUTE not defined\n");
#endif

#ifndef	MSG_OOB
	printf("+ MSG_OOB not defined\n");
#endif

#ifndef	MSG_PEEK
	printf("+ MSG_PEEK not defined\n");
#endif

#ifndef	MSG_WAITALL
	printf("+ MSG_WAITALL not defined\n");
#endif
}

/*
 * Check whether we can use sendto() and recvfrom() with a TCP socket.
 * Use a different length for each output function, so if it does work,
 * we can see it with tcpdump and separate it from the other outputs.
 */

void
sendto_01()
{
	int		sockfd, n;
	socklen_t	len;

	sockfd = TcpSockByAddr("140.252.13.34", 7);		/* echo server */

	/*
	 * This also verifies that we can call sendto() on a TCP socket
	 * if we don't specify a destination address.
	 */

	Sendto(sockfd, "hello", 5, 0, NULL, NULL);

	if ( (n = Recvfrom(sockfd, buff, sizeof(buff), 0, NULL, NULL)) != 5)
		err_quit("! Recvfrom expected 5");

	/*
	 * Now see what happens when we ask for the server's address.
	 * Berkeley-derived implementations do not return this (p. 517, tcpipiv2)
	 * while Solaris does.
	 */

	Sendto(sockfd, "world", 5, 0, NULL, NULL);

	len = sizeof(servaddr) * 2;	/* that's a lie */
	if ( (n = Recvfrom(sockfd, buff, sizeof(buff), 0,
					   (SA *) &servaddr, &len)) != 5)
		err_quit("! Recvfrom expected 5");
	if (len != 0) {
		err_msg("+ recvfrom on TCP socket returns len = %d for sender's addr",
				len);
		if (len == sizeof(servaddr))
			printf("  recvfrom from %s, port %d\n",
				   inet_ntoa(servaddr.sin_addr), ntohs(servaddr.sin_port));
	}

	Close(sockfd);

	/*
	 * Now try and specify a destination address for sendto() on
	 * a TCP socket.
	 */

	sockfd = TcpSockByAddr("140.252.13.34", 7);		/* echo server */

		/* should not work with destination address specified */
	n = sendto(sockfd, "hello1", 6, 0, (SA *) &servaddr, sizeof(servaddr));
	if (n < 0)
		err_ret("sendto on TCP socket specifying dest addr returns error");
	else if (n == 6)
#ifdef	MSG_EOF		/* defined only if T/TCP supported */
		err_msg("+ sendto on TCP socket specifying dest addr OK (T/TCP supported)");
#else
		err_msg("+ sendto on TCP socket specifying dest addr OK");
#endif
	else
		err_quit("! sendto on TCP socket specifying dest addr, n = %d", n);

	Close(sockfd);

	/*
	 * Now an unconnected UDP socket.
	 */

	sockfd = UdpSockByAddr("140.252.13.34", 7);		/* echo server */

		/* should not work */
	if ( (n = sendto(sockfd, "hello12", 7, 0, (SA *) 0, 0)) >= 0)
		err_msg("+ sendto on unconnected UDP without dest addr OK, n = %d", n);
	else if (errno != EDESTADDRREQ)
		err_ret("+ sendto on unconnected UDP without dest addr, unexpected errno");

		/* should not work */
	if ( (n = write(sockfd, "hello", 7)) >= 0)
		err_msg("+ write on unconnected UDP OK, n = %d", n);
	else if (errno != EDESTADDRREQ)
		err_ret("+ write on unconnected UDP, unexpected errno");

	Close(sockfd);

	/*
	 * Now a connected UDP socket.
	 */

	sockfd = UdpConnSockByAddr("140.252.13.34", 7);		/* echo server */

		/* should work */
	if ( (n = write(sockfd, "hello123", 8)) < 0)
		err_sys("! write on connected UDP, n = %d", n);
	else if (n != 8)
		err_quit("! write on connected UDP, n = %d", n);

		/* should work */
	if ( (n = sendto(sockfd, "hello1234", 9, 0, (SA *) 0, 0)) < 0)
		err_sys("! sendto on connected UDP without dest addr, n = %d", n);
	else if (n != 9)
		err_quit("! sendto on connected UDP without dest addr, n = %d", n);

		/* should not work */
	n = sendto(sockfd, "hello12345", 10, 0, (SA *) &servaddr, sizeof(servaddr));
	if (n < 0 && errno != EISCONN)
		err_ret("+ sendto on connected UDP with dest addr, unexpected errno");
	else if (n >= 0)
		err_msg("+ sendto on connected UDP with dest addr OK, n = %d", n);

	Close(sockfd);
}

/*
 * Send a UDP datagram to a server at IP address 1, and look at
 * the return address in the response.  If the server is multihomed,
 * the return address can differ from our original destination address.
 */

void
udp_01()
{
}

static void
usage(const char *msg)
{
	err_msg(
"options: -v    verbose\n"
);

	if (msg[0] != 0)
		err_quit("%s", msg);
	exit(1);
}

int
main(int argc, char **argv)
{
	int		c;

	opterr = 0;		/* don't want getopt() writing to stderr */
	while ( (c = getopt(argc, argv, "v")) != -1) {
		switch (c) {
		case 'v':
			verbose = 1;
			break;

		case '?':
			usage("unrecognized option");
		}
	}

	if (verbose) printf("header_flags\n");
	header_flags();

	if (verbose) printf("udp_01\n");
	udp_01();

	if (verbose) printf("sendto_01\n");
	sendto_01();
}
