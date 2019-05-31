#include	"gai_hdr.h"
#include	<sys/utsname.h>

#ifdef	UNIXdomain
/* include ga_unix */
int
ga_unix(const char *path, struct addrinfo *hintsp, struct addrinfo **result)
{
	int				rc;
	struct addrinfo	*aihead, **aipnext;

	aihead = NULL;
	aipnext = &aihead;

	if (hintsp->ai_family != AF_UNSPEC && hintsp->ai_family != AF_LOCAL)
		return(EAI_ADDRFAMILY);

	if (hintsp->ai_socktype == 0) {
			/* 4no socket type specified: return stream then dgram */
		hintsp->ai_socktype = SOCK_STREAM;
		if ( (rc = ga_aistruct(&aipnext, hintsp, path, AF_LOCAL)) != 0)
			return(rc);
		hintsp->ai_socktype = SOCK_DGRAM;
	}

	if ( (rc = ga_aistruct(&aipnext, hintsp, path, AF_LOCAL)) != 0)
		return(rc);

	if (hintsp->ai_flags & AI_CANONNAME) {
		struct utsname	myname;

		if (uname(&myname) < 0)
			return(EAI_SYSTEM);
		if ( (aihead->ai_canonname = strdup(myname.nodename)) == NULL)
			return(EAI_MEMORY);
	}

	*result = aihead;	/* pointer to first structure in linked list */
	return(0);
}
/* end ga_unix */
#endif	/* UNIXdomain */
