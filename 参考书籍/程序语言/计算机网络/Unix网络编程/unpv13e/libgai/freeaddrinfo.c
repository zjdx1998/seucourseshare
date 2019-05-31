#include	"gai_hdr.h"

/* include freeaddrinfo */
void
freeaddrinfo(struct addrinfo *aihead)
{
	struct addrinfo	*ai, *ainext;

	for (ai = aihead; ai != NULL; ai = ainext) {
		if (ai->ai_addr != NULL)
			free(ai->ai_addr);		/* socket address structure */

		if (ai->ai_canonname != NULL)
			free(ai->ai_canonname);

		ainext = ai->ai_next;	/* can't fetch ai_next after free() */
		free(ai);				/* the addrinfo{} itself */
	}
}
/* end freeaddrinfo */
