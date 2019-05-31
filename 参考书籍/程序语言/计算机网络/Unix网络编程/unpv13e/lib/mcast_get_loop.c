#include	"unp.h"

int
mcast_get_loop(int sockfd)
{
	switch (sockfd_to_family(sockfd)) {
	case AF_INET: {
		u_char		flag;
		socklen_t	len;

		len = sizeof(flag);
		if (getsockopt(sockfd, IPPROTO_IP, IP_MULTICAST_LOOP,
					   &flag, &len) < 0)
			return(-1);
		return(flag);
	}

#ifdef	IPV6
	case AF_INET6: {
		u_int		flag;
		socklen_t	len;

		len = sizeof(flag);
		if (getsockopt(sockfd, IPPROTO_IPV6, IPV6_MULTICAST_LOOP,
					   &flag, &len) < 0)
			return(-1);
		return(flag);
	}
#endif

	default:
		errno = EAFNOSUPPORT;
		return(-1);
	}
}

int
Mcast_get_loop(int sockfd)
{
	int		rc;

	if ( (rc = mcast_get_loop(sockfd)) < 0)
		err_sys("mcast_get_loop error");
	return(rc);
}
