#include	"unp.h"

int
sock_get_port(const struct sockaddr *sa, socklen_t salen)
{
	switch (sa->sa_family) {
	case AF_INET: {
		struct sockaddr_in	*sin = (struct sockaddr_in *) sa;

		return(sin->sin_port);
	}

#ifdef	IPV6
	case AF_INET6: {
		struct sockaddr_in6	*sin6 = (struct sockaddr_in6 *) sa;

		return(sin6->sin6_port);
	}
#endif
	}

    return(-1);		/* ??? */
}
