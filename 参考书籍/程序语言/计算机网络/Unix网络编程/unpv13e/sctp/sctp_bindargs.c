#include	"unp.h"

int
sctp_bind_arg_list(int sock_fd, char **argv, int argc)
{
	struct addrinfo *addr;
	char *bindbuf, *p, portbuf[10];
	int addrcnt=0;
	int i;

	bindbuf = (char *)Calloc(argc, sizeof(struct sockaddr_storage));
	p = bindbuf;
	sprintf(portbuf, "%d", SERV_PORT);
	for( i=0; i<argc; i++ ) {
		addr = Host_serv(argv[i], portbuf, AF_UNSPEC, SOCK_SEQPACKET);
		memcpy(p, addr->ai_addr, addr->ai_addrlen);
		freeaddrinfo(addr);
		addrcnt++;
		p += addr->ai_addrlen;
	}
	Sctp_bindx(sock_fd,(SA *)bindbuf,addrcnt,SCTP_BINDX_ADD_ADDR);
	free(bindbuf);
	return(0);
}
