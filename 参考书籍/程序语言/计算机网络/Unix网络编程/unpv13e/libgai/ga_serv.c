#include	"gai_hdr.h"

/*
 * This function handles the service string.
 */

/* include ga_serv */
int
ga_serv(struct addrinfo *aihead, const struct addrinfo *hintsp,
		const char *serv)
{
	int				port, rc, nfound;
	struct servent	*sptr;

	nfound = 0;
	if (isdigit(serv[0])) {		/* check for port number string first */
		port = htons(atoi(serv));
		if (hintsp->ai_socktype) {
				/* 4caller specifies socket type */
			if ( (rc = ga_port(aihead, port, hintsp->ai_socktype)) < 0)
				return(EAI_MEMORY);
			nfound += rc;
		} else {
				/* 4caller does not specify socket type */
			if ( (rc = ga_port(aihead, port, SOCK_STREAM)) < 0)
				return(EAI_MEMORY);
			nfound += rc;
			if ( (rc = ga_port(aihead, port, SOCK_DGRAM)) < 0)
				return(EAI_MEMORY);
			nfound += rc;
		}
	} else {
			/* 4try service name, TCP then UDP */
		if (hintsp->ai_socktype == 0 || hintsp->ai_socktype == SOCK_STREAM) {
			if ( (sptr = getservbyname(serv, "tcp")) != NULL) {
				if ( (rc = ga_port(aihead, sptr->s_port, SOCK_STREAM)) < 0)
					return(EAI_MEMORY);
				nfound += rc;
			}
		}
		if (hintsp->ai_socktype == 0 || hintsp->ai_socktype == SOCK_DGRAM) {
			if ( (sptr = getservbyname(serv, "udp")) != NULL) {
				if ( (rc = ga_port(aihead, sptr->s_port, SOCK_DGRAM)) < 0)
					return(EAI_MEMORY);
				nfound += rc;
			}
		}
	}

	if (nfound == 0) {
		if (hintsp->ai_socktype == 0)
			return(EAI_NONAME);	/* all calls to getservbyname() failed */
		else
			return(EAI_SERVICE);/* service not supported for socket type */
	}
	return(0);
}
/* end ga_serv */
