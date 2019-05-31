#include	"unp.h"

int
main(int argc, char **argv)
{
	int				sockfd, flag;
	ssize_t			n;
	char			buff[MAXLINE];
	const int		on = 1;
	socklen_t		addrlen, len;
	struct sockaddr	*cliaddr;

	if (argc == 2)
		sockfd = Udp_server(NULL, argv[1], &addrlen);
	else if (argc == 3)
		sockfd = Udp_server(argv[1], argv[2], &addrlen);
	else
		err_quit("usage: test01 <hostname> <service>");

	cliaddr = Malloc(addrlen);

#ifdef	IP_RECVDSTADDR
	Setsockopt(sockfd, IPPROTO_IP, IP_RECVDSTADDR, &on, sizeof(on));

	len = sizeof(flag);
	Getsockopt(sockfd, IPPROTO_IP, IP_RECVDSTADDR, &flag, &len);
	printf("IP_RECVDSTADDR option = %d, size = %d\n", flag, len);
#else
	printf("IP_RECVDSTADDR not defined\n");
#endif

#ifdef	IP_RECVIF
	Setsockopt(sockfd, IPPROTO_IP, IP_RECVIF, &on, sizeof(on));

	len = sizeof(flag);
	Getsockopt(sockfd, IPPROTO_IP, IP_RECVIF, &flag, &len);
	printf("IP_RECVIF option = %d, size = %d\n", flag, len);
#else
	printf("IP_RECVIF not defined\n");
#endif

	for ( ; ; ) {
#define	CONTROL_LEN	(sizeof(struct cmsghdr) + sizeof(struct in_addr))
		char		control[CONTROL_LEN];

		msg.msg_control = control;
		msg.msg_controllen = sizeof(control);
		msg.msg_flags = 0;
#else
		bzero(&msg, sizeof(msg));	/* make certain msg_accrightslen = 0 */
#endif

		msg.msg_name = sa;
		msg.msg_namelen = *salenptr;
		iov[0].iov_base = ptr;
		iov[0].iov_len = nbytes;
		msg.msg_iov = iov;
		msg.msg_iovlen = 1;

		if ( (n = recvmsg(fd, &msg, *flagsp)) < 0)
			return(n);

		len = addrlen;
		n = Recvfrom(sockfd, buff, MAXLINE, 0, cliaddr, &len);
		printf("datagram from %s\n", Sock_ntop(cliaddr, len));

        ticks = time(NULL);
        snprintf(buff, sizeof(buff), "%.24s\r\n", ctime(&ticks));
        Sendto(sockfd, buff, strlen(buff), 0, cliaddr, len);
	}
	exit(0);
}
