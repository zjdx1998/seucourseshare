#include	"unp.h"
#include	<ctype.h>		/* isxdigit(), etc. */
#include	<arpa/nameser.h>
#include	<resolv.h>		/* res_init, _res */

		/* following internal flag cannot overlap with other AI_xxx flags */
#define	AI_CLONE	     4	/* clone this entry for other socket types */

struct search {
  const char	*host;	/* hostname of address string */
  int			family;	/* AF_xxx */
};

		/* function prototypes for our own internal functions */
static int	ga_echeck(const char *, const char *, const struct addrinfo *);
static int	ga_nsearch(const char *, const struct addrinfo *,
					   struct search *);
static int	ga_aistruct(struct addrinfo ***, const struct addrinfo *,
						void *, int);
static int	ga_serv(struct addrinfo *, const struct addrinfo *, const char *);
static int	ga_port(struct addrinfo *, int , int);
static int	ga_unix(const char *, struct addrinfo *, struct addrinfo **);
static struct addrinfo	*ga_clone(struct addrinfo *);

		/* globals for all functions in this file; these *must* be
		   read-only if this function is to be reentrant */
static struct addrinfo	hints_default;

int
getaddrinfo(const char *hostname, const char *servname,
			const struct addrinfo *hintsp, struct addrinfo **result)
{
	int					rc, error, nsearch;
	char				**ap;
	struct hostent		*hptr;
	struct search		search[3], *sptr;
	struct addrinfo		hints, *ai, *aihead, **aipnext;

	/*
	 * If we encounter an error we want to free() any dynamic memory
	 * that we've allocated.  This is our hack to simplify the code.
	 */
#define	error(e) { error = (e); goto bad; }

	if (hintsp == NULL) {
		hints = hints_default;	/* struct copy */
		hints.ai_family = AF_UNSPEC;
	} else
		hints = *hintsp;		/* struct copy */

		/* 4first some basic error checking */
	if ( (rc = ga_echeck(hostname, servname, &hints)) != 0)
		error(rc);

#ifdef	UNIXDOMAIN
	/*
	 * Special case Unix domain first;
	 * remainder of function for IPv4/IPv6.
	 */
	if (hostname != NULL && hostname[0] == '/' &&
	    (servname == NULL || servname[0] == '\0'))
		return(ga_unix(hostname, &hints, result));

	if (servname != NULL && servname[0] == '/' &&
	    (hostname == NULL || hostname[0] == '\0'))
		return(ga_unix(servname, &hints, result));
#endif

	nsearch = ga_nsearch(hostname, &hints, &search[0]);

	aihead = NULL;
	aipnext = &aihead;
	for (sptr = &search[0]; sptr < &search[nsearch]; sptr++) {
#ifdef	IPV4
			/* 4check for an IPv4 dotted-decimal string */
		if (isdigit(sptr->host[0])) {
			struct in_addr	inaddr;

			if (inet_pton(AF_INET, sptr->host, &inaddr) == 1) {
				rc = ga_aistruct(&aipnext, &hints, &inaddr, AF_INET);
				if (rc != 0)
					error(rc);
				continue;
			}
		}
#endif
	
#ifdef	IPV6
			/* 4check for an IPv6 hex string */
		if (isxdigit(sptr->host[0]) || sptr->host[0] == ':') {
			struct in6_addr	in6addr;

			if (inet_pton(AF_INET6, sptr->host, &in6addr) == 1) {
				rc = ga_aistruct(&aipnext, &hints, &in6addr, AF_INET6);
				if (rc != 0)
					error(rc);
				continue;
			}
		}
#endif
			/* 4look up hostname */
		if ((_res.options & RES_INIT) == 0)
			res_init();			/* need this to set _res.options */

		if (nsearch == 2)
			hptr = gethostbyname2(sptr->host, sptr->family);
		else {
#ifdef	IPV6
			if (sptr->family == AF_INET6)
				_res.options |= RES_USE_INET6;
			else
				_res.options &= ~RES_USE_INET6;
#endif
			hptr = gethostbyname(sptr->host);
		}
		if (hptr == NULL) {
			switch (h_errno) {
				case HOST_NOT_FOUND:	error(EAI_NONAME);
				case TRY_AGAIN:			error(EAI_AGAIN);
				case NO_RECOVERY:		error(EAI_FAIL);
				case NO_DATA:			error(EAI_NODATA);
				default:				error(EAI_NONAME);
			}
		}
	
			/* 4check for address family mismatch if one specified */
		if (hints.ai_family != AF_UNSPEC && hints.ai_family != hptr->h_addrtype)
			error(EAI_ADDRFAMILY);
	
			/* 4create one addrinfo{} for each returned address */
		for (ap = hptr->h_addr_list; *ap != NULL; ap++) {
			if ( (ai = calloc(1, sizeof(struct addrinfo))) == NULL)
				error(EAI_MEMORY);
			*aipnext = ai;
			aipnext = &ai->ai_next;
			rc = ga_aistruct(&aipnext, &hints, *ap, hptr->h_addrtype);
			if (rc != 0)
				error(rc);
		}
	}
	/* "aihead" points to the first structure in the linked list */

	if (hostname != NULL && hostname[0] != '\0' &&
		hints.ai_flags & AI_CANONNAME) {
		aihead->ai_canonname = strdup(hptr->h_name != NULL ?
									  hptr->h_name : search[0].host);
		if (aihead->ai_canonname == NULL)
			error(EAI_MEMORY);
	}

		/* 4now process the service name */
	if (servname != NULL && servname[0] != '\0') {
		if ( (rc = ga_serv(aihead, &hints, servname)) != 0)
			error(rc);
	}

	*result = aihead;	/* pointer to first structure in linked list */
	return(0);

bad:
	freeaddrinfo(aihead);	/* free any alloc'ed memory */
	return(error);
}

/*
 * Basic error checking at the beginning.
 */

static int
ga_echeck(const char *hostname, const char *servname,
		  const struct addrinfo *hintsp)
{
	if (hintsp->ai_flags & ~(AI_PASSIVE | AI_CANONNAME))
		return(EAI_BADFLAGS);	/* unknown flag bits */

	if (hostname == NULL || hostname[0] == '\0') {
		if (servname == NULL || servname[0] == '\0')
			return(EAI_NONAME);	/* host or service must be specified */
	}

	switch(hintsp->ai_family) {
		case AF_UNSPEC:
			break;
#ifdef	IPV4
		case AF_INET:
			if (hintsp->ai_socktype != 0 &&
				(hintsp->ai_socktype != SOCK_STREAM &&
				 hintsp->ai_socktype != SOCK_DGRAM &&
				 hintsp->ai_socktype != SOCK_RAW))
				return(EAI_SOCKTYPE);	/* invalid socket type */
			break;
#endif
#ifdef	IPV6
		case AF_INET6:
			if (hintsp->ai_socktype != 0 &&
				(hintsp->ai_socktype != SOCK_STREAM &&
				 hintsp->ai_socktype != SOCK_DGRAM &&
				 hintsp->ai_socktype != SOCK_RAW))
				return(EAI_SOCKTYPE);	/* invalid socket type */
			break;
#endif
#ifdef	UNIXDOMAIN
		case AF_LOCAL:
			if (hintsp->ai_socktype != 0 &&
				(hintsp->ai_socktype != SOCK_STREAM &&
				 hintsp->ai_socktype != SOCK_DGRAM))
				return(EAI_SOCKTYPE);	/* invalid socket type */
			break;
#endif
		default:
			return(EAI_FAMILY);		/* unknown protocol family */
	}
	return(0);
}

/*
 * Set up the search[] array with the hostnames and address families
 * that we are to look up.
 */

static int
ga_nsearch(const char *hostname, const struct addrinfo *hintsp,
		   struct search *search)
{
	int		nsearch = 0;

	if (hostname == NULL || hostname[0] == '\0') {
		if (hintsp->ai_flags & AI_PASSIVE) {
				/* 4no hostname and AI_PASSIVE: implies wildcard bind */
			switch (hintsp->ai_family) {
#ifdef	IPV4
			case AF_INET:
				search[nsearch].host = "0.0.0.0";
				search[nsearch].family = AF_INET;
				nsearch++;
				break;
#endif
#ifdef	IPV6
			case AF_INET6:
				search[nsearch].host = "0::0";
				search[nsearch].family = AF_INET6;
				nsearch++;
				break;
#endif
			case AF_UNSPEC:
#ifdef	IPV6
				search[nsearch].host = "0::0";	/* IPv6 first, then IPv4 */
				search[nsearch].family = AF_INET6;
				nsearch++;
#endif
#ifdef	IPV4
				search[nsearch].host = "0.0.0.0";
				search[nsearch].family = AF_INET;
				nsearch++;
#endif
				break;
			}
		} else {
				/* 4no host and not AI_PASSIVE: connect to local host */
			switch (hintsp->ai_family) {
#ifdef	IPV4
			case AF_INET:
				search[nsearch].host = "localhost";	/* 127.0.0.1 */
				search[nsearch].family = AF_INET;
				nsearch++;
				break;
#endif
#ifdef	IPV6
			case AF_INET6:
				search[nsearch].host = "0::1";
				search[nsearch].family = AF_INET6;
				nsearch++;
				break;
#endif
			case AF_UNSPEC:
#ifdef	IPV6
				search[nsearch].host = "0::1";	/* IPv6 first, then IPv4 */
				search[nsearch].family = AF_INET6;
				nsearch++;
#endif
#ifdef	IPV4
				search[nsearch].host = "localhost";
				search[nsearch].family = AF_INET;
				nsearch++;
#endif
				break;
			}
		}
	} else {	/* host is specified */
		switch (hintsp->ai_family) {
#ifdef	IPV4
		case AF_INET:
			search[nsearch].host = hostname;
			search[nsearch].family = AF_INET;
			nsearch++;
			break;
#endif
#ifdef	IPV6
		case AF_INET6:
			search[nsearch].host = hostname;
			search[nsearch].family = AF_INET6;
			nsearch++;
			break;
#endif
		case AF_UNSPEC:
#ifdef	IPV6
			search[nsearch].host = hostname;
			search[nsearch].family = AF_INET6;	/* IPv6 first */
			nsearch++;
#endif
#ifdef	IPV4
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

/*
 * Create and fill in a addrinfo{}.
 */

static int
ga_aistruct(struct addrinfo ***paipnext, const struct addrinfo *hintsp,
			void *addr, int family)
{
	struct addrinfo	*ai;

	if ( (ai = calloc(1, sizeof(struct addrinfo))) == NULL)
		return(EAI_MEMORY);
	ai->ai_next = NULL;
	ai->ai_canonname = NULL;
	**paipnext = ai;
	*paipnext = &ai->ai_next;

	if ( (ai->ai_socktype = hintsp->ai_socktype) == 0)
		ai->ai_flags |= AI_CLONE;

	ai->ai_protocol = hintsp->ai_protocol;
	
	switch ((ai->ai_family = family)) {
#ifdef	IPV4
		case AF_INET: {
			struct sockaddr_in	*sinptr;

				/* 4allocate sockaddr_in{} and fill in all but port */
			if ( (sinptr = calloc(1, sizeof(struct sockaddr_in))) == NULL)
				return(EAI_MEMORY);
#ifdef	HAVE_SOCKADDR_SA_LEN
			sinptr->sin_len = sizeof(struct sockaddr_in);
#endif
			sinptr->sin_family = AF_INET;
			memcpy(&sinptr->sin_addr, addr, sizeof(struct in_addr));
			ai->ai_addr = (struct sockaddr *) sinptr;
			ai->ai_addrlen = sizeof(struct sockaddr_in);
			break;
		}
#endif	/* IPV4 */
	
#ifdef	IPV6
				/* 4allocate sockaddr_in6{} and fill in all but port */
		case AF_INET6: {
			struct sockaddr_in6	*sin6ptr;

			if ( (sin6ptr = calloc(1, sizeof(struct sockaddr_in6))) == NULL)
				return(EAI_MEMORY);
#ifdef	HAVE_SOCKADDR_SA_LEN
			sin6ptr->sin6_len = sizeof(struct sockaddr_in6);
#endif
			sin6ptr->sin6_family = AF_INET6;
			memcpy(&sin6ptr->sin6_addr, addr, sizeof(struct in6_addr));
			ai->ai_addr = (struct sockaddr *) sin6ptr;
			ai->ai_addrlen = sizeof(struct sockaddr_in6);
			break;
		}
#endif	/* IPV6 */
	}
	return(0);
}

/*
 * This function handles the service string.
 */

static int
ga_serv(struct addrinfo *aihead, const struct addrinfo *hintsp,
		const char *serv)
{
	int				port, rc, nfound;
	struct servent	*sptr;

		/* 4check for port number first */
	if (isdigit(serv[0]) && hintsp->ai_socktype != 0) {
		port = htons(atoi(serv));
		if ( (rc = ga_port(aihead, port, hintsp->ai_socktype)) == 0)
			return(EAI_NONAME);
		else if (rc < 0)
			return(EAI_MEMORY);
		else
			return(0);
	}

		/* 4try TCP first */
	nfound = 0;
	if (hintsp->ai_socktype == 0 || hintsp->ai_socktype == SOCK_STREAM) {
		if ( (sptr = getservbyname(serv, "tcp")) != NULL) {
			if ( (rc = ga_port(aihead, sptr->s_port, SOCK_STREAM)) < 0)
				return(EAI_MEMORY);
			nfound += rc;
		}
	}

		/* 4try UDP */
	if (hintsp->ai_socktype == 0 || hintsp->ai_socktype == SOCK_DGRAM) {
		if ( (sptr = getservbyname(serv, "udp")) != NULL) {
			if ( (rc = ga_port(aihead, sptr->s_port, SOCK_DGRAM)) < 0)
				return(EAI_MEMORY);
			nfound += rc;
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

static int
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
#ifdef	IPV4
			case AF_INET:
				((struct sockaddr_in *) ai->ai_addr)->sin_port = port;
				nfound++;
				break;
#endif
#ifdef	IPV6
			case AF_INET6:
				((struct sockaddr_in6 *) ai->ai_addr)->sin6_port = port;
				nfound++;
				break;
#endif
		}
	}
	return(nfound);
}

/*
 * Clone a new addrinfo structure from an existing one.
 */

static struct addrinfo *
ga_clone(struct addrinfo *ai)
{
	struct addrinfo	*new;

	if ( (new = calloc(1, sizeof(struct addrinfo))) == NULL)
		return(NULL);

	new->ai_next = ai->ai_next;
	ai->ai_next = new;

	new->ai_flags = 0;				/* make sure AI_CLONE is off */
	new->ai_family = ai->ai_family;
	new->ai_socktype = ai->ai_socktype;
	new->ai_protocol = ai->ai_protocol;
	new->ai_canonname = NULL;
	new->ai_addrlen = ai->ai_addrlen;
	if ( (new->ai_addr = malloc(ai->ai_addrlen)) == NULL)
		return(NULL);
	memcpy(new->ai_addr, ai->ai_addr, ai->ai_addrlen);

	return(new);
}

#ifdef	UNIXDOMAIN
/*
 * Do everything for a Unix domain socket.
 * Only one addrinfo{} is returned.
 */

static int
ga_unix(const char *path, struct addrinfo *hintsp, struct addrinfo **result)
{
	struct addrinfo		*ai;
	struct sockaddr_un	*unp;

	if (hintsp->ai_socktype == 0)
		return(EAI_SOCKTYPE);	/* we cannot tell socket type from service */

	if ( (ai = calloc(1, sizeof(struct addrinfo))) == NULL)
		return(NULL);

	ai->ai_flags = 0;
	ai->ai_family = AF_LOCAL;
	ai->ai_socktype = hintsp->ai_socktype;
	ai->ai_protocol = 0;

		/* allocate and fill in a socket address structure */
	ai->ai_addrlen = sizeof(struct sockaddr_un);
	if ( (ai->ai_addr = malloc(ai->ai_addrlen)) == NULL)
		return(EAI_MEMORY);
	unp = (struct sockaddr_un *) ai->ai_addr;
	unp->sun_family = AF_UNIX;
	strncpy(unp->sun_path, path, sizeof(unp->sun_path));

	ai->ai_canonname = NULL;	/* maybe return the i-node number :-) */
	ai->ai_next = NULL;
	*result = ai;

	if (hintsp->ai_flags & AI_PASSIVE)
		unlink(path);		/* OK if this fails */

	return(0);		/* success */
}
#endif	/* UNIXDOMAIN */

void
freeaddrinfo(struct addrinfo *aihead)
{
	struct addrinfo	*ai, *ainext;

	for (ai = aihead; ai != NULL; ai = ainext) {
		if (ai->ai_addr != NULL)
			free(ai->ai_addr);		/* the socket address structure */
		if (ai->ai_canonname != NULL)
			free(ai->ai_canonname);	/* the canonical name */
		ainext = ai->ai_next;		/* can't fetch ai_next after free() */
		free(ai);					/* the addrinfo{} itself */
	}
}
