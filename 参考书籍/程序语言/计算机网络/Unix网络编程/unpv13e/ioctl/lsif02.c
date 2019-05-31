#include	"unp.h"

#include	<net/if.h>
#ifdef	HAVE_SOCKADDR_DL_STRUCT
#include	<net/if_dl.h>
#include	<net/if_types.h>
#endif

int
main(int argc, char **argv)
{
	int					sockfd, len;
	char				*ptr, buf[2048], addrstr[INET_ADDRSTRLEN];
	struct ifconf		ifc;
	struct ifreq		*ifr;
	struct sockaddr_in	*sinptr;

	if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
		err_sys("socket error");

	ifc.ifc_len = sizeof(buf);
	ifc.ifc_req = (struct ifreq *) buf;
	if (ioctl(sockfd, SIOCGIFCONF, &ifc) < 0)
		err_sys("ioctl SIOCGIFCONF error");

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

#ifdef	AF_INET6
		case AF_INET6: {
			struct sockaddr_in6	*sin6ptr;
			char addr6str[INET6_ADDRSTRLEN];

			sin6ptr = (struct sockaddr_in6 *) &ifr->ifr_addr;
			printf("%s\t%s\n", ifr->ifr_name,
				   Inet_ntop(AF_INET6, &sin6ptr->sin6_addr, addr6str, sizeof(addr6str)));
			break;
		}
#endif

#ifdef	HAVE_SOCKADDR_DL_STRUCT
		case AF_LINK: {
			struct sockaddr_dl	*sdlptr;
			char				str[18];
			char				*etherprint(const u_char *, char *);

			sdlptr = (struct sockaddr_dl *) &ifr->ifr_addr;
			printf("%s", ifr->ifr_name);
			if (sdlptr->sdl_index)
				printf("\t<link %d>", sdlptr->sdl_index);
			if (sdlptr->sdl_type == IFT_ETHER && sdlptr->sdl_alen)
				printf("\t%s", etherprint((u_char *) LLADDR(sdlptr), str));
			putchar('\n');
			break;
		}
#endif

		default:
			printf("%s\n", ifr->ifr_name);
			break;
		}
	}
	exit(0);
}

#ifdef	HAVE_SOCKADDR_DL_STRUCT
char *
etherprint(const u_char eaddr[6], char string[18])
{
	snprintf(string, 18, "%02x:%02x:%02x:%02x:%02x:%02x",
		    eaddr[0], eaddr[1], eaddr[2], eaddr[3], eaddr[4], eaddr[5] );
	string[17] = '\0';
	return(string);
}
#endif
