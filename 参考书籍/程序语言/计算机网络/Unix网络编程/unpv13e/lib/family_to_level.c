#include "unp.h"

int
family_to_level(int family)
{
	switch (family) {
	case AF_INET:
		return IPPROTO_IP;
#ifdef	IPV6
	case AF_INET6:
		return IPPROTO_IPV6;
#endif
	default:
		return -1;
	}
}

int
Family_to_level(int family)
{
	int		rc;

	if ( (rc = family_to_level(family)) < 0)
		err_sys("family_to_level error");

	return(rc);
}
