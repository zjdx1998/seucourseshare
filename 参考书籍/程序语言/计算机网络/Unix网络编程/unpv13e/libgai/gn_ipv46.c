#include	"gai_hdr.h"

/*
 * Handle either an IPv4 or an IPv6 address and port.
 */

/* include gn_ipv46 */
int
gn_ipv46(char *host, size_t hostlen, char *serv, size_t servlen,
		 void *aptr, size_t alen, int family, int port, int flags)
{
	char			*ptr;
	struct hostent	*hptr;
	struct servent	*sptr;

	if (hostlen > 0) {
		if (flags & NI_NUMERICHOST) {
			if (inet_ntop(family, aptr, host, hostlen) == NULL)
				return(1);
		} else {
			hptr = gethostbyaddr(aptr, alen, family);
			if (hptr != NULL && hptr->h_name != NULL) {
				if (flags & NI_NOFQDN) {
					if ( (ptr = strchr(hptr->h_name, '.')) != NULL)
						*ptr = 0;	/* overwrite first dot */
				}
				snprintf(host, hostlen, "%s", hptr->h_name);
			} else {
				if (flags & NI_NAMEREQD)
					return(1);
				if (inet_ntop(family, aptr, host, hostlen) == NULL)
					return(1);
			}
		}
	}

	if (servlen > 0) {
		if (flags & NI_NUMERICSERV) {
			snprintf(serv, servlen, "%d", ntohs(port));
		} else {
			sptr = getservbyport(port, (flags & NI_DGRAM) ? "udp" : NULL);
			if (sptr != NULL && sptr->s_name != NULL)
				snprintf(serv, servlen, "%s", sptr->s_name);
			else
				snprintf(serv, servlen, "%d", ntohs(port));
		}
	}
	return(0);
}
/* end gn_ipv46 */
