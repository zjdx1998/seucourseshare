#include	"gai_hdr.h"

/*
 * Basic error checking of flags, family, socket type, and protocol.
 */

/* include ga_echeck */
int
ga_echeck(const char *hostname, const char *servname,
		  int flags, int family, int socktype, int protocol)
{
	if (flags & ~(AI_PASSIVE | AI_CANONNAME))
		return(EAI_BADFLAGS);	/* unknown flag bits */

	if (hostname == NULL || hostname[0] == '\0') {
		if (servname == NULL || servname[0] == '\0')
			return(EAI_NONAME);	/* host or service must be specified */
	}

	switch(family) {
		case AF_UNSPEC:
			break;
#ifdef	IPv4
		case AF_INET:
			if (socktype != 0 &&
				(socktype != SOCK_STREAM &&
				 socktype != SOCK_DGRAM &&
				 socktype != SOCK_RAW))
				return(EAI_SOCKTYPE);	/* invalid socket type */
			break;
#endif
#ifdef	IPv6
		case AF_INET6:
			if (socktype != 0 &&
				(socktype != SOCK_STREAM &&
				 socktype != SOCK_DGRAM &&
				 socktype != SOCK_RAW))
				return(EAI_SOCKTYPE);	/* invalid socket type */
			break;
#endif
#ifdef	UNIXdomain
		case AF_LOCAL:
			if (socktype != 0 &&
				(socktype != SOCK_STREAM &&
				 socktype != SOCK_DGRAM))
				return(EAI_SOCKTYPE);	/* invalid socket type */
			break;
#endif
		default:
			return(EAI_FAMILY);		/* unknown protocol family */
	}
	return(0);
}
/* end ga_echeck */
