#include	"gai_hdr.h"

/*
 * Clone a new addrinfo structure from an existing one.
 */

/* include ga_clone */
struct addrinfo *
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
/* end ga_clone */
