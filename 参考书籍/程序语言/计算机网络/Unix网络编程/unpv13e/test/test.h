#include	"unp.h"

	/* globals */
extern struct sockaddr_in	servaddr, cliaddr;
extern char		buff[8192];
extern int		verbose;

int		TcpSockByAddr(char *, int);
int		UdpSockByAddr(char *, int);
int		UdpConnSockByAddr(char *, int);
