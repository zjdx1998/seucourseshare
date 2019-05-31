#include	"gai_hdr.h"

/*
 * Go through all the addrinfo structures, checking for a match of the
 * socket type and filling in the socket type, and then the port number
 * in the corresponding socket address structures.
 *
 * The AI_CLONE flag works as follows.  Consider a multihomed host with
 * two IP addresses and no socket type specified by the caller.  After
 * the "host" search there are two addrinfo structures, one per IP address.
 * Assuming a service supported by both TCP and UDP (say the daytime
 * service) we need to return *four* addrinfo structures:
 *		IP#1, SOCK_STREAM, TCP port,
 *		IP#1, SOCK_DGRAM, UDP port,
 *		IP#2, SOCK_STREAM, TCP port,
 *		IP#2, SOCK_DGRAM, UDP port.
 * To do this, when the "host" loop creates an addrinfo structure, if the
 * caller has not specified a socket type (hintsp->ai_socktype == 0), the
 * AI_CLONE flag is set.  When the following function finds an entry like
 * this it is handled as follows: If the entry's ai_socktype is still 0,
 * this is the first use of the structure, and the ai_socktype field is set.
 * But, if the entry's ai_socktype is nonzero, then we clone a new addrinfo
 * structure and set it's ai_socktype to the new value.  Although we only
 * need two socket types today (SOCK_STREAM and SOCK_DGRAM) this algorithm
 * will handle any number.  Also notice that Posix.1g requires all socket
 * types to be nonzero.
 */

/* include ga_port */
int
ga_port(struct addrinfo *aihead, int port, int socktype)
		/* port must be in network byte order */
{
	int				nfound = 0;
	struct addrinfo	*ai;

	for (ai = aihead; ai != NULL; ai = ai->ai_next) {
		if (ai->ai_flags & AI_CLONE) {
			if (ai->ai_socktype != 0) {
				if ( (ai = ga_clone(ai)) == NULL)
					return(-1);		/* memory allocation error */
				/* ai points to newly cloned entry, which is what we want */
			}
		} else if (ai->ai_socktype != socktype)
			continue;		/* ignore if mismatch on socket type */

		ai->ai_socktype = socktype;

		switch (ai->ai_family) {
#ifdef	IPv4
			case AF_INET:
				((struct sockaddr_in *) ai->ai_addr)->sin_port = port;
				nfound++;
				break;
#endif
#ifdef	IPv6
			case AF_INET6:
				((struct sockaddr_in6 *) ai->ai_addr)->sin6_port = port;
				nfound++;
				break;
#endif
		}
	}
	return(nfound);
}
/* end ga_port */
