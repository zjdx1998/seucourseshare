#include	"unp.h"

int
mcast_get_if(int sockfd)
{
	switch (sockfd_to_family(sockfd)) {
	case AF_INET: {
		/* TODO: similar to mcast_set_if() */
		return(-1);
	}

#ifdef	IPV6
	case AF_INET6: {
		u_int		idx;
		socklen_t	len;

		len = sizeof(idx);
		if (getsockopt(sockfd, IPPROTO_IPV6, IPV6_MULTICAST_IF,
					   &idx, &len) < 0)
			return(-1);
		return(idx);
	}
#endif

	default:
		errno = EAFNOSUPPORT;
		return(-1);
	}
}

int
Mcast_get_if(int sockfd)
{
	int		rc;

	if ( (rc = mcast_get_if(sockfd)) < 0)
		err_sys("mcast_get_if error");
	return(rc);
}
