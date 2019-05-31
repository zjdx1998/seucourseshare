#include	"gai_hdr.h"

#ifdef	UNIXDOMAIN
int
ga_unixstruct(const char *path, struct addrinfo *hintsp,
			  struct addrinfo **result, int socktype)
{
	struct addrinfo		*ai;
	struct sockaddr_un	*unp;

	if ( (ai = calloc(1, sizeof(struct addrinfo))) == NULL)
		return(EAI_MEMORY);

	ai->ai_flags = 0;
	ai->ai_family = AF_LOCAL;
	ai->ai_socktype = socktype;
	ai->ai_protocol = 0;

		/* allocate and fill in a socket address structure */
	ai->ai_addrlen = sizeof(struct sockaddr_un);
	if ( (ai->ai_addr = malloc(ai->ai_addrlen)) == NULL)
		return(EAI_MEMORY);
	unp = (struct sockaddr_un *) ai->ai_addr;
	unp->sun_family = AF_UNIX;
	strncpy(unp->sun_path, path, sizeof(unp->sun_path));

	ai->ai_canonname = NULL;
	ai->ai_next = NULL;
	*result = ai;

	if (hintsp->ai_flags & AI_PASSIVE)
		unlink(path);		/* OK if this fails */

	return(0);		/* success */
}
#endif	/* UNIXDOMAIN */
