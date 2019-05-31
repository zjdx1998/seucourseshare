/*
 * Copyright (c) 1996 W. Richard Stevens.  All rights reserved. 
 *
 * Permission to use or modify this software for educational or
 * for commercial purposes, and without fee, is hereby granted,
 * provided that the above copyright notice appears in connection
 * with any and all uses, with clear indication as to any
 * modifications made.  The author RESERVES the sole rights of
 * reproduction, publication and distribution and hence permission
 * to print this source code in any book, reference manual,
 * magazine, or other type of publication, including any digital
 * medium, must be granted in writing by W. Richard Stevens.
 *
 * The author makes no representations about the suitability of this 
 * software for any purpose.  It is provided "as is" without express
 * or implied warranty. 
 */

/* tabs set for 4 spaces, not 8 */

#ifdef	__osf__
#define	_SOCKADDR_LEN		/* required for this implementation */
#define	INET6				/* required for this implementation */
#endif

#include	<sys/types.h>
#include	<sys/socket.h>
#include	<netinet/in.h>
#include	<ctype.h>		/* isdigit() */
#include	<netdb.h>		/* hostent{}, servent{}, etc. */
#include	<arpa/inet.h>	/* inet_pton() */
#include	<arpa/nameser.h>
#include	<resolv.h>		/* DNS resolver */
#include	<stdlib.h>		/* malloc() and calloc() */
#include	<string.h>		/* strdup() and strncpy() */
#include	<unistd.h>		/* unlink() */
#include	<sys/un.h>		/* for Unix domain socket stuff */

#ifndef	__osf__
#include	"addrinfo.h"	/* defines in here really belong in <netdb.h> */
#endif

/* NOTE: this code assumes you have the inet_pton() function as defined
 * in the BIND-4.9.4 release or later (ftp://ftp.vix.com/pub/bind).
 * If you don't have this in your resolver library, in this tar file is
 * a copy of it, along with inet_ntop(). */

/* We need a way to determine if the compiling host supports IPv4/IPv6.
 * Cannot test for AF_INET6, as some vendors #define it, even though
 * they don't support it. */

#ifdef	AF_INET
#define	IPV4	/* this is tested throughout the code that follows */
#endif

			/* no one constant that all implementations define, sigh */
#if		defined(IPV6ADDR_ANY_INIT)
#define	IPV6	/* this is tested throughout the code that follows */
#elif	defined(IPV6_FLOWINFO_FLOWLABEL)
#define	IPV6	/* this is tested throughout the code that follows */
#endif

/* There is no requirement that getaddrinfo() support Unix domain sockets,
 * but what the heck, it doesn't take too much code, and it's a real good
 * test whether an application is *really* protocol-independent. */

#ifdef	AF_UNIX
#define	LOCAL	/* this is tested throughout the code that follows */

#ifndef	AF_LOCAL
#define	AF_LOCAL	AF_UNIX		/* the Posix.1g correct term */
#endif
#ifndef	PF_LOCAL
#define	PF_LOCAL	PF_UNIX		/* the Posix.1g correct term */
#endif

#endif	/* AF_UNIX */

/* Define REENTRANT if the reentrant versions of get{host|serv}byname
 * are to be used.
 * Note that getaddrinfo{} *must* be reentrant if the underlying
 * get{host|serv}byname_r functions are provided.
 * This program has only been tested with the Solaris 2.x functions.
 * (I have no idea if other vendors have the same functions or not.)
 *
 * Long diatribe: Don't define REENTRANT.  At least not until you know
 * what your vendor's gethostbyname_r() function does with regard to
 * IPv4/IPv6 addresses.  If you really need a reentrant version of this
 * function, because you call it from different threads, then use a
 * mutex lock to protect the calls.
 * The problem at the time of this writing is the handling of IPv4/IPv6
 * addresses.  BIND-4.9.4 does it the "right" way :-), but this won't
 * be documented until a later revision of the IPv6 BSD API spec.  Also
 * BIND-4.9.4 doesn't provide the reentrant _r() functions, and I have
 * no idea what the vendors like Sun have done with these functions.
 * The code far below that calls gethostbyname() sets the resolver
 * RES_USE_INET6 option if the caller specifies an ai_family of AF_INET6.
 * This causes 16-byte addresses to be returned, regardless, either
 * "true" IPv6 address from AAAA records, or IPv4-mapped IPv6 addresses
 * from A records.  If the caller specifies an ai_family of AF_INET6,
 * then we should return 16-byte addresses.
 * With BIND-4.9.4 the caller can also force the return of 16-byte addresses
 * by setting the environment variable RES_OPTIONS, as in
 *    % RES_OPTIONS=inet6 ./a.out arguments...
 * This way the caller need not pollute the code with code like
 * ai_family = AF_INET6, making the code protocol-dependent.
 */

/* #define	REENTRANT */

/* Define following function prototype if your headers don't define it */

int	 inet_pton(int, const char *, void *);

#define	HENTBUFSIZ	8*1024
#define	HENTMAXADDR	    32	/* max binary address: 16 for IPv4, 24 for IPv6 */

		/* following internal flags cannot overlap with other AI_xxx flags */
#define	AI_CLONE	     4	/* clone this entry for other socket types */

		/* function prototypes for our own internal functions */
static int	getaddrinfo_host(const char *, struct hostent *,
							 struct hostent **, char *, int, int);
static int	getaddrinfo_serv(struct addrinfo *,
							 const struct addrinfo *, const char *,
							 struct servent *, char *, int);
static int	getaddrinfo_port(struct addrinfo *, int , int);
static int	addrinfo_local(const char *, struct addrinfo *,
						   struct addrinfo **);
static struct addrinfo	*getaddrinfo_clone(struct addrinfo *);

		/* globals for all functions in this file; these *must* be
		   read-only if this function is to be reentrant */
static struct addrinfo	hints_default;

int
getaddrinfo(const char *host, const char *serv,
			const struct addrinfo *hintsptr, struct addrinfo **result)
{
	int					rc, error;
	struct hostent		*hptr, hent;
	struct servent		sent;
	char				hentbuf[HENTBUFSIZ], hent_addr[HENTMAXADDR];
	char				*hent_aliases[1], *hent_addr_list[2];
	char				**ap;
	struct addrinfo		hints, *ai, *aihead, **aipnext;

#ifdef	IPV4
	struct sockaddr_in	*sinptr;
#endif

#ifdef	IPV6
	struct sockaddr_in6	*sin6ptr;
#endif

/* If we encounter an error we want to free() any dynamic memory
   that we've allocated.  This is our hack to simplify the code. */
#define	error(e) { error = (e); goto bad; }

	/*
	 * We must make a copy of the caller's hints structure, so we can
	 * modify ai_family.  If the caller doesn't provide a hints structure,
	 * use a default one.  This simplifies all the following code.
	 * In the default one, ai_flags, ai_socktype, and ai_protocol are all 0,
	 * but we have to set ai_family to AF_UNSPEC, which isn't guaranteed to
	 * be 0.
	 */ 
	if (hintsptr == NULL) {
		hints_default.ai_family = AF_UNSPEC;
		hints = hints_default;	/* struct copy */
	} else
		hints = *hintsptr;		/* struct copy */

	/*
	 * First some error checking.
	 */
	if (hints.ai_flags & ~(AI_PASSIVE | AI_CANONNAME))
		error(EAI_BADFLAGS);	/* unknown flag bits */

	/*
	 * Check that the family is valid, and if a socket type is also
	 * specified, check that it's valid for the family.
	 */
	if (hints.ai_family != 0) {
		switch(hints.ai_family) {
			case AF_UNSPEC:	break;
				/* Actually, AF_UNSPEC is normally defined as 0,
				   but Posix.1g does not require this. */
#ifdef	IPV4
			case AF_INET:
				if (hints.ai_socktype != 0 &&
					(hints.ai_socktype != SOCK_STREAM &&
					 hints.ai_socktype != SOCK_DGRAM &&
					 hints.ai_socktype != SOCK_RAW))
					error(EAI_SOCKTYPE);	/* invalid socket type */
				break;
#endif
#ifdef	IPV6
			case AF_INET6:
				if (hints.ai_socktype != 0 &&
					(hints.ai_socktype != SOCK_STREAM &&
					 hints.ai_socktype != SOCK_DGRAM &&
					 hints.ai_socktype != SOCK_RAW))
					error(EAI_SOCKTYPE);	/* invalid socket type */
				break;
#endif
#ifdef	LOCAL
			case AF_LOCAL:
				if (hints.ai_socktype != 0 &&
					(hints.ai_socktype != SOCK_STREAM &&
					 hints.ai_socktype != SOCK_DGRAM))
					error(EAI_SOCKTYPE);	/* invalid socket type */
				break;
#endif
		default:
			error(EAI_FAMILY);		/* unknown protocol family */
		}
	}

	if (host == NULL || host[0] == '\0') {
		if (serv == NULL || serv[0] == '\0')
			error(EAI_NONAME);	/* either host or serv must be specified */

		if (hints.ai_flags & AI_PASSIVE) {
			/*
			 * No "host" and AI_PASSIVE: the returned address must be
			 * ready for bind(): 0.0.0.0 for IPv4 or 0::0 for IPv6.
			 */
			switch (hints.ai_family) {
#ifdef	IPV4
			case AF_INET:	host = "0.0.0.0"; break;
#endif
#ifdef	IPV6
			case AF_INET6:	host = "0::0"; break;
#endif
#ifdef	LOCAL
			case AF_LOCAL:
				if (serv[0] != '/')		/* allow service to specify path */
					error(EAI_ADDRFAMILY);
				break;
#endif
			case 0:			error(EAI_ADDRFAMILY);
					/* How can we initialize a socket address structure
					   for a passive open if we don't even know the family? */
			}
		} else {
			/*
			 * No host and not AI_PASSIVE: caller implies connect() to
			 * local host.
			 */

			host = "localhost";
		}
	} else if (hints.ai_family == 0) {
		/*
		 * Caller specifies a host but no address family.
		 * If the host string is really a valid IPv4 dotted-decimal address,
		 * set family to IPv4.  Similarly for IPv6 strings.
		 * This allows server applications to be protocol independent
		 * (not having to hard code a protocol family), allowing the
		 * user who starts the program to specify either 0.0.0.0 or 0::0.
		 *
		 * Assumed below is that inet_pton() allows only "valid" strings,
		 * which Paul Vixie put into the BIND-4.9.4 version of this function.
		 */

		char	temp[16];

#ifdef	IPV4
		if (inet_pton(AF_INET, host, temp) == 1)
			hints.ai_family = AF_INET;
#endif
#ifdef	IPV6
		if (inet_pton(AF_INET6, host, temp) == 1)
			hints.ai_family = AF_INET6;
#endif
		/*
		 * Note that we could bypass some of the testing done in
		 * getaddrinfo_host(), but it doesn't seem worth complicating
		 * this (already long) function.
		 */
	}

#ifdef	LOCAL
	/*
	 * For a Unix domain socket only one string can be provided and we
	 * require it to be an absolute pathname.  (Using relative pathnames
	 * is asking for trouble.)  We allow this string to be specified as
	 * either the hostname or the service name, in which case we ignore
	 * the other string.  Notice that a slash is not allowed in a DNS
	 * hostname (see RFC 1912) and a slash does not appear in any of the
	 * service names in /etc/services either.  Hence no conflict.
	 * For example, often a protocol-independent server will allow an
	 * argument to specify the service (e.g., port number) and let the
	 * hostname be wildcarded.  Similarly, a protocol-independent client
	 * often allows only the hostname as a command-line argument, hardcoding
	 * a service name in the program (which we ignore).
	 */

	if ((host != NULL && host[0] == '/'))
		return(addrinfo_local(host, &hints, result));

	if ((serv != NULL && serv[0] == '/'))
		return(addrinfo_local(serv, &hints, result));
#endif

	/*
	 * Look up the host.  The code above guarantees that "host"
	 * is a nonnull pointer to a nonull string.
	 *
	 * We first initialize "hent" assuming "host" is an IPv4/IPv6 address
	 * (instead of a name).  This saves passing lots of additional
	 * arguments to getaddrinfo_host().
	 */

	hent.h_name = hentbuf;		/* char string specifying address goes here */
	hent.h_aliases = hent_aliases;
	hent_aliases[0] = NULL;			/* no aliases */
	hent.h_addr_list = hent_addr_list;
	hent_addr_list[0] = hent_addr;	/* one binary address in [0] */
	hent_addr_list[1] = NULL;
	hptr = &hent;

	if ( (rc = getaddrinfo_host(host, &hent, &hptr, hentbuf, HENTBUFSIZ,
								hints.ai_family)) != 0)
		error(rc);

	/*
	 * "hptr" now points to a filled in hostent{}.
	 * If "host" was an IPv4/IPv6 address, instead of a name, then
	 * "hptr" points to our own "hent" structure.
	 * If gethostbyname_r() was called, then "hptr" points to our own
	 * "hent" structure, which was passed as as an argument to the
	 * reentrant function.
	 * If gethostbyname() was called, then "hptr" points to the static
	 * hostent{} that it returned.
	 *
	 * Check for address family mismatch if the caller specified one.
	 * Note that Posix.1g assumes that AF_foo == PF_foo.
	 */
	if (hints.ai_family != AF_UNSPEC && hints.ai_family != hptr->h_addrtype)
		error(EAI_ADDRFAMILY);

	/*
	 * Go through the list of returned addresses and create one
	 * addrinfo{} for each one, linking all the structures together.
	 * We still have not looked at the service--that comes after this.
	 */

	aihead = NULL;
	aipnext = &aihead;
	for (ap = hptr->h_addr_list; *ap != NULL; ap++) {
		if ( (ai = calloc(1, sizeof(struct addrinfo))) == NULL)
			error(EAI_MEMORY);
		*aipnext = ai;			/* prev points to this new one */
		aipnext = &ai->ai_next;	/* pointer to next one goes here */

			/* initialize from hints; could be 0 */
		if ( (ai->ai_socktype = hints.ai_socktype) == 0)
			ai->ai_flags |= AI_CLONE;
		ai->ai_protocol = hints.ai_protocol;

		ai->ai_family = hptr->h_addrtype;
		switch (ai->ai_family) {
			/*
			 * Allocate a socket address structure and fill it in.
			 * The port number will be filled in later, when the service
			 * is processed.
			 */
#ifdef	IPV4
			case AF_INET:
				if ( (sinptr = calloc(1, sizeof(struct sockaddr_in))) == NULL)
					error(EAI_MEMORY);
#ifdef	SIN_LEN
				sinptr->sin_len = sizeof(struct sockaddr_in);
#endif
				sinptr->sin_family = AF_INET;
				memcpy(&sinptr->sin_addr, *ap, sizeof(struct in_addr));
				ai->ai_addr = (struct sockaddr *) sinptr;
				ai->ai_addrlen = sizeof(struct sockaddr_in);
				break;
#endif	/* IPV4 */

#ifdef	IPV6
			case AF_INET6:
				if ( (sin6ptr = calloc(1, sizeof(struct sockaddr_in6))) == NULL)
					error(EAI_MEMORY);
#ifdef	SIN6_LEN
				sin6ptr->sin6_len = sizeof(struct sockaddr_in6);
#endif
				sin6ptr->sin6_family = AF_INET6;
				memcpy(&sin6ptr->sin6_addr, *ap, sizeof(struct in6_addr));
				ai->ai_addr = (struct sockaddr *) sin6ptr;
				ai->ai_addrlen = sizeof(struct sockaddr_in6);
				break;
#endif	/* IPV6 */
		}
	}
	/* "aihead" points to the first structure in the linked list */

	if (hints.ai_flags & AI_CANONNAME) {
		/*
		 * Posix.1g doesn't say what to do if this flag is set and
		 * multiple addrinfo structures are returned.
		 * We return the canonical name only in the first addrinfo{}.
		 */
		if (hptr->h_name != NULL) {
			if ( (aihead->ai_canonname = strdup(hptr->h_name)) == NULL)
				error(EAI_MEMORY);
		} else {
			/*
			 * Posix.1g says we can just set ai_canonname to point to the
			 * "host" argument, but that makes freeaddrinfo() harder.
			 * We dynamically allocate room for a copy of "host".
			 */
			if ( (aihead->ai_canonname = strdup(host)) == NULL)
				error(EAI_MEMORY);
		}
	}

	/*
	 * Now look up the service, if specified.
	 */
	if (serv != NULL && serv[0] != '\0') {
		if ( (rc = getaddrinfo_serv(aihead, &hints, serv, &sent,
									hentbuf, HENTBUFSIZ)) != 0)
			error(rc);
	}

	*result = aihead;	/* pointer to first structure in linked list */
	return(0);

bad:
	freeaddrinfo(aihead);	/* free any alloc'ed memory */
	return(error);
}

/*
 * This function handles the host string.
 */

static int
getaddrinfo_host(const char *host,
				 struct hostent *hptr, struct hostent **hptrptr,
				 char *buf, int bufsiz, int family)
{

#ifdef	REENTRANT
	int		h_errno;
#endif	/* REENTRANT */

#ifdef	IPV4
	/*
	 * We explicitly check for an IPv4 dotted-decimal string.
	 * Recent versions of gethostbyname(), starting around BIND 4.9.2
	 * do this too, but we have the check here so we don't depend on
	 * this newer feature.  (You wouldn't believe the ancient versions
	 * of BIND that some vendors ship.)
	 */
	if (isdigit(host[0])) {
		if (inet_pton(AF_INET, host, hptr->h_addr_list[0]) == 1) {
				/* Success.  Finish making up the hostent{} as though
				   we had called gethostbyname(). */
			strncpy(hptr->h_name, host, bufsiz-1);
			buf[bufsiz-1] = '\0';
			hptr->h_addrtype = AF_INET;
			hptr->h_length = sizeof(struct in_addr);
			return(0);
		}
	}
#endif	/* IPV4 */

#ifdef	IPV6
	/*
	 * Check for an IPv6 hex string.
	 */
	if (isxdigit(host[0]) || host[0] == ':') {
		if (inet_pton(AF_INET6, host, hptr->h_addr_list[0]) == 1) {
				/* Success.  Finish making up a hostent{} as though
				   we had called gethostbyname(). */
			strncpy(buf, host, bufsiz-1);
			buf[bufsiz-1] = '\0';
			hptr->h_addrtype = AF_INET6;
			hptr->h_length = sizeof(struct in6_addr);
			return(0);
		}
	}
#endif	/* IPV6 */

	/*
	 * Not an address, must be a hostname, try the DNS.
	 * Initialize the resolver, if not already initialized.
	 */

	if ((_res.options & RES_INIT) == 0)
		res_init();			/* need this to set _res.options below */

#ifdef	IPV6
	/*
	 * Notice that the following might be considered optional, and
	 * could be #ifdef'ed out if your <resolv.h> does not define
	 * RES_USE_INET6.  But I am assuming you have BIND-4.9.4 installed
	 * and want the IPv4/IPv6 semantics that it defines for gethostbyname().
	 */

#ifndef	RES_USE_INET6
	/* This is a gross hack; following line from BIND-4.9.4 release ... */
	/* (if you're using 4.9.4, but have not installed the include files) */
#define	RES_USE_INET6	0x00002000	/* use/map IPv6 in gethostbyname() */
#endif
	if (family == AF_INET6)
		_res.options |= RES_USE_INET6;
#endif	/* IPV6 */

#ifdef	REENTRANT
	hptr = gethostbyname_r(host, hptr, buf, bufsiz, &h_errno);
#else
	hptr = gethostbyname(host);
#endif	/* REENTRANT */
	if (hptr == NULL) {
		switch (h_errno) {
			case HOST_NOT_FOUND:	return(EAI_NONAME);
			case TRY_AGAIN:			return(EAI_AGAIN);
			case NO_RECOVERY:		return(EAI_FAIL);
			case NO_DATA:			return(EAI_NODATA);
			default:				return(EAI_NONAME);
		}
	}
	*hptrptr = hptr;
	return(0);
}

/*
 * This function handles the service string.
 */

static int
getaddrinfo_serv(struct addrinfo *aihead,
				 const struct addrinfo *hintsptr, const char *serv,
				 struct servent *sptrarg, char *buf, int bufsiz)
{
	int				port, rc;
	int				nfound = 0;
	struct servent	*sptr;

	/*
	 * We allow the service to be a numeric string, which we
	 * interpret as a decimal port number.  Posix.1g doesn't
	 * explicitly say to do this, but it just makes sense.
	 * But to do this the caller must specify a socket type,
	 * else there's no way to return values for socket().
	 */

	if (isdigit(serv[0]) && hintsptr->ai_socktype != 0) {
		port = htons(atoi(serv));
		if ( (rc = getaddrinfo_port(aihead, port, hintsptr->ai_socktype)) == 0)
			return(EAI_NONAME);
		else if (rc < 0)
			return(EAI_MEMORY);
		else
			return(0);
	}

	/*
	 * Not a special case, try the "/etc/services" file (or whatever).
	 * We first try TCP, if applicable.
	 */

	if (hintsptr->ai_socktype == 0 || hintsptr->ai_socktype == SOCK_STREAM) {
#ifdef	REENTRANT
		sptr = getservbyname_r(serv, "tcp", sptrarg, buf, bufsiz);
#else
		sptr = getservbyname(serv, "tcp");
#endif	/* REENTRANT */
		if (sptr != NULL) {
			rc = getaddrinfo_port(aihead, sptr->s_port, SOCK_STREAM);
			if (rc < 0)
				return(EAI_MEMORY);
			nfound += rc;
		}
	}

	/*
	 * Now try UDP, if applicable.
	 */
	if (hintsptr->ai_socktype == 0 || hintsptr->ai_socktype == SOCK_DGRAM) {
#ifdef	REENTRANT
		sptr = getservbyname_r(serv, "udp", sptrarg, buf, bufsiz);
#else
		sptr = getservbyname(serv, "udp");
#endif	/* REENTRANT */
		if (sptr != NULL) {
			rc = getaddrinfo_port(aihead, sptr->s_port, SOCK_DGRAM);
			if (rc < 0)
				return(EAI_MEMORY);
			nfound += rc;
		}
	}

	if (nfound == 0) {
			/* You could call getservbyname() one more time, with no
			   protocol specified, but "tcp" and "udp" are all that
			   are supported today. */

		if (hintsptr->ai_socktype == 0)
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
 * caller has not specified a socket type (hints->ai_socktype == 0), the
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
getaddrinfo_port(struct addrinfo *aihead, int port, int socktype)
		/* port must be in network byte order */
{
	int				nfound = 0;
	struct addrinfo	*ai;

	for (ai = aihead; ai != NULL; ai = ai->ai_next) {
		/*
		 * We set the socket type but not the protocol, because if a
		 * port number is specified, the protocol must be TCP or UDP,
		 * and a protocol of 0 for socket() is fine for TCP and UDP.
		 * The only time a nonzero protocol argument is required by
		 * socket() is for a raw socket, in which case a service will
		 * not be specified to getaddrinfo().
		 */

		if (ai->ai_flags & AI_CLONE) {
			if (ai->ai_socktype != 0) {
				if ( (ai = getaddrinfo_clone(ai)) == NULL)
					return(-1);	/* tell caller it's a memory allocation error */
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
getaddrinfo_clone(struct addrinfo *ai)
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

#ifdef	LOCAL
/*
 * Do everything for a Unix domain socket.
 * Only one addrinfo{} is returned.
 */

static int
addrinfo_local(const char *path, struct addrinfo *hints,
			   struct addrinfo **result)
{
	struct addrinfo		*ai;
	struct sockaddr_un	*unp;

	if (hints->ai_socktype == 0)
		return(EAI_SOCKTYPE);	/* we cannot tell socket type from service */

	if ( (ai = calloc(1, sizeof(struct addrinfo))) == NULL)
		return(NULL);

	ai->ai_flags = 0;
	ai->ai_family = AF_LOCAL;
	ai->ai_socktype = hints->ai_socktype;
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

	if (hints->ai_flags & AI_PASSIVE)
		unlink(path);		/* OK if this fails */

	return(0);		/* success */
}
#endif	/* LOCAL */

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
