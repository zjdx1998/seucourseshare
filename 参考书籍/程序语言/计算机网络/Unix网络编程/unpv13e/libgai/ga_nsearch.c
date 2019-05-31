#include	"gai_hdr.h"

/*
 * Set up the search[] array with the hostnames and address families
 * that we are to look up.
 */

/* include ga_nsearch1 */
int
ga_nsearch(const char *hostname, const struct addrinfo *hintsp,
		   struct search *search)
{
	int		nsearch = 0;

	if (hostname == NULL || hostname[0] == '\0') {
		if (hintsp->ai_flags & AI_PASSIVE) {
				/* 4no hostname and AI_PASSIVE: implies wildcard bind */
			switch (hintsp->ai_family) {
#ifdef	IPv4
			case AF_INET:
				search[nsearch].host = "0.0.0.0";
				search[nsearch].family = AF_INET;
				nsearch++;
				break;
#endif
#ifdef	IPv6
			case AF_INET6:
				search[nsearch].host = "0::0";
				search[nsearch].family = AF_INET6;
				nsearch++;
				break;
#endif
			case AF_UNSPEC:
#ifdef	IPv6
				search[nsearch].host = "0::0";	/* IPv6 first, then IPv4 */
				search[nsearch].family = AF_INET6;
				nsearch++;
#endif
#ifdef	IPv4
				search[nsearch].host = "0.0.0.0";
				search[nsearch].family = AF_INET;
				nsearch++;
#endif
				break;
			}
/* end ga_nsearch1 */
/* include ga_nsearch2 */
		} else {
				/* 4no host and not AI_PASSIVE: connect to local host */
			switch (hintsp->ai_family) {
#ifdef	IPv4
			case AF_INET:
				search[nsearch].host = "localhost";	/* 127.0.0.1 */
				search[nsearch].family = AF_INET;
				nsearch++;
				break;
#endif
#ifdef	IPv6
			case AF_INET6:
				search[nsearch].host = "0::1";
				search[nsearch].family = AF_INET6;
				nsearch++;
				break;
#endif
			case AF_UNSPEC:
#ifdef	IPv6
				search[nsearch].host = "0::1";	/* IPv6 first, then IPv4 */
				search[nsearch].family = AF_INET6;
				nsearch++;
#endif
#ifdef	IPv4
				search[nsearch].host = "localhost";
				search[nsearch].family = AF_INET;
				nsearch++;
#endif
				break;
			}
		}
/* end ga_nsearch2 */
/* include ga_nsearch3 */
	} else {	/* host is specified */
		switch (hintsp->ai_family) {
#ifdef	IPv4
		case AF_INET:
			search[nsearch].host = hostname;
			search[nsearch].family = AF_INET;
			nsearch++;
			break;
#endif
#ifdef	IPv6
		case AF_INET6:
			search[nsearch].host = hostname;
			search[nsearch].family = AF_INET6;
			nsearch++;
			break;
#endif
		case AF_UNSPEC:
#ifdef	IPv6
			search[nsearch].host = hostname;
			search[nsearch].family = AF_INET6;	/* IPv6 first */
			nsearch++;
#endif
#ifdef	IPv4
			search[nsearch].host = hostname;
			search[nsearch].family = AF_INET;	/* then IPv4 */
			nsearch++;
#endif
			break;
		}
	}
	if (nsearch < 1 || nsearch > 2)
		err_quit("nsearch = %d", nsearch);
	return(nsearch);
}
/* end ga_nsearch3 */
