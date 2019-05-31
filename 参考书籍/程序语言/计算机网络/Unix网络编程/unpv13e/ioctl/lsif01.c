#include	"unp.h"

#include	<net/if.h>

int
main(int argc, char **argv)
{
	int					sockfd, len;
	char				*ptr, buf[2048], addrstr[INET_ADDRSTRLEN];
	struct ifconf		ifc;
	struct ifreq		*ifr;
	struct sockaddr_in	*sinptr;

	sockfd = Socket(AF_INET, SOCK_DGRAM, 0);

	ifc.ifc_len = sizeof(buf);
	ifc.ifc_req = (struct ifreq *) buf;
	Ioctl(sockfd, SIOCGIFCONF, &ifc);

	for (ptr = buf; ptr < buf + ifc.ifc_len; ) {
		ifr = (struct ifreq *) ptr;
		len = sizeof(struct sockaddr);
#ifdef	HAVE_SOCKADDR_SA_LEN
		if (ifr->ifr_addr.sa_len > len)
			len = ifr->ifr_addr.sa_len;		/* length > 16 */
#endif
		ptr += sizeof(ifr->ifr_name) + len;	/* for next one in buffer */

		switch (ifr->ifr_addr.sa_family) {
		case AF_INET:
			sinptr = (struct sockaddr_in *) &ifr->ifr_addr;
			printf("%s\t%s\n", ifr->ifr_name,
				   Inet_ntop(AF_INET, &sinptr->sin_addr, addrstr, sizeof(addrstr)));
			break;

		default:
			printf("%s\n", ifr->ifr_name);
			break;
		}
	}
	exit(0);
}
