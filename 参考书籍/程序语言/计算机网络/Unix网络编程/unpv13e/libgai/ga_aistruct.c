#include	"gai_hdr.h"

/*
 * Create and fill in an addrinfo{}.
 */

/* include ga_aistruct1 */
int
ga_aistruct(struct addrinfo ***paipnext, const struct addrinfo *hintsp,
			const void *addr, int family)
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
/* end ga_aistruct1 */
	
/* include ga_aistruct2 */
	switch ((ai->ai_family = family)) {
#ifdef	IPv4
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
#ifdef	IPv6
		case AF_INET6: {
			struct sockaddr_in6	*sin6ptr;

				/* 4allocate sockaddr_in6{} and fill in all but port */
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
#ifdef	UNIXdomain
		case AF_LOCAL: {
			struct sockaddr_un	*unp;

				/* 4allocate sockaddr_un{} and fill in */
/* *INDENT-OFF* */
			if (strlen(addr) >= sizeof(unp->sun_path))
				return(EAI_SERVICE);
			if ( (unp = calloc(1, sizeof(struct sockaddr_un))) == NULL)
				return(EAI_MEMORY);
/* *INDENT-ON* */
			unp->sun_family = AF_LOCAL;
			strcpy(unp->sun_path, addr);
#ifdef	HAVE_SOCKADDR_SA_LEN
			unp->sun_len = SUN_LEN(unp);
#endif
			ai->ai_addr = (struct sockaddr *) unp;
			ai->ai_addrlen = sizeof(struct sockaddr_un);
			if (hintsp->ai_flags & AI_PASSIVE)
				unlink(unp->sun_path);	/* OK if this fails */
			break;
		}
#endif	/* UNIXDOMAIN */
	}
	return(0);
}
/* end ga_aistruct2 */
