#include	"gai_hdr.h"

/* include getnameinfo */
int
getnameinfo(const struct sockaddr *sa, socklen_t salen,
		    char *host, size_t hostlen,
			char *serv, size_t servlen, int flags)
{

	switch (sa->sa_family) {
#ifdef	IPv4
	case AF_INET: {
		struct sockaddr_in	*sain = (struct sockaddr_in *) sa;

		return(gn_ipv46(host, hostlen, serv, servlen,
						&sain->sin_addr, sizeof(struct in_addr),
						AF_INET, sain->sin_port, flags));
	}
#endif

#ifdef	IPv6
	case AF_INET6: {
		struct sockaddr_in6	*sain = (struct sockaddr_in6 *) sa;

		return(gn_ipv46(host, hostlen, serv, servlen,
						&sain->sin6_addr, sizeof(struct in6_addr),
						AF_INET6, sain->sin6_port, flags));
	}
#endif

#ifdef	UNIXdomain
	case AF_LOCAL: {
		struct sockaddr_un	*un = (struct sockaddr_un *) sa;

		if (hostlen > 0)
			snprintf(host, hostlen, "%s", "/local");
		if (servlen > 0)
			snprintf(serv, servlen, "%s", un->sun_path);
		return(0);
	}
#endif

	default:
		return(1);
	}
}
/* end getnameinfo */
