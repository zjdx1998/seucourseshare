#include	"unp.h"

#ifdef	HAVE_SOCKADDR_DL_STRUCT
#include	<net/if_dl.h>
#endif

int
sock_cmp_port(const struct sockaddr *sa1, const struct sockaddr *sa2,
			 socklen_t salen)
{
	if (sa1->sa_family != sa2->sa_family)
		return(-1);

	switch (sa1->sa_family) {
	case AF_INET: {
		return( ((struct sockaddr_in *) sa1)->sin_port ==
				((struct sockaddr_in *) sa2)->sin_port);
	}

#ifdef	IPV6
	case AF_INET6: {
		return( ((struct sockaddr_in6 *) sa1)->sin6_port ==
				((struct sockaddr_in6 *) sa2)->sin6_port);
	}
#endif

	}
    return (-1);
}
