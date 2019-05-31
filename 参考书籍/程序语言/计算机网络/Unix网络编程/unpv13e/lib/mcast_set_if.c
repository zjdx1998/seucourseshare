#include	"unp.h"
#include	<net/if.h>

int
mcast_set_if(int sockfd, const char *ifname, u_int ifindex)
{
	switch (sockfd_to_family(sockfd)) {
	case AF_INET: {
		struct in_addr		inaddr;
		struct ifreq		ifreq;

		if (ifindex > 0) {
			if (if_indextoname(ifindex, ifreq.ifr_name) == NULL) {
				errno = ENXIO;	/* i/f index not found */
				return(-1);
			}
			goto doioctl;
		} else if (ifname != NULL) {
			strncpy(ifreq.ifr_name, ifname, IFNAMSIZ);
doioctl:
			if (ioctl(sockfd, SIOCGIFADDR, &ifreq) < 0)
				return(-1);
			memcpy(&inaddr,
				   &((struct sockaddr_in *) &ifreq.ifr_addr)->sin_addr,
				   sizeof(struct in_addr));
		} else
			inaddr.s_addr = htonl(INADDR_ANY);	/* remove prev. set default */

		return(setsockopt(sockfd, IPPROTO_IP, IP_MULTICAST_IF,
						  &inaddr, sizeof(struct in_addr)));
	}

#ifdef	IPV6
	case AF_INET6: {
		u_int	idx;

		if ( (idx = ifindex) == 0) {
			if (ifname == NULL) {
				errno = EINVAL;	/* must supply either index or name */
				return(-1);
			}
			if ( (idx = if_nametoindex(ifname)) == 0) {
				errno = ENXIO;	/* i/f name not found */
				return(-1);
			}
		}
		return(setsockopt(sockfd, IPPROTO_IPV6, IPV6_MULTICAST_IF,
						  &idx, sizeof(idx)));
	}
#endif

	default:
		errno = EAFNOSUPPORT;
		return(-1);
	}
}

void
Mcast_set_if(int sockfd, const char *ifname, u_int ifindex)
{
	if (mcast_set_if(sockfd, ifname, ifindex) < 0)
		err_sys("mcast_set_if error");
}
