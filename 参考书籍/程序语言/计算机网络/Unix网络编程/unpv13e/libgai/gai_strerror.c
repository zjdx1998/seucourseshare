/*
 * Return a string containing some additional information after an
 * error from getaddrinfo().
 */

#include	<sys/types.h>
#include	"addrinfo.h"	/* XXX should be <netdb.h> */

char *
gai_strerror(int err)
{
	switch (err) {
		case EAI_ADDRFAMILY:return("address family for host not supported");
		case EAI_AGAIN:		return("temporary failure in name resolution");
		case EAI_BADFLAGS:	return("invalid flags value");
		case EAI_FAIL:		return("non-recoverable failure in name resolution");
		case EAI_FAMILY:	return("address family not supported");
		case EAI_MEMORY:	return("memory allocation failure");
		case EAI_NODATA:	return("no address associated with host");
		case EAI_NONAME:	return("host nor service provided, or not known");
		case EAI_SERVICE:	return("service not supported for socket type");
		case EAI_SOCKTYPE:	return("socket type not supported");
		case EAI_SYSTEM:	return("system error");
		default:			return("unknown getaddrinfo() error");
	}
}
