/* include net_rt_dump */
#include	"unproute.h"

char *
net_rt_dump(int family, int flags, size_t *lenp)
{
	int		mib[6];
	char	*buf;

	mib[0] = CTL_NET;
	mib[1] = AF_ROUTE;
	mib[2] = 0;
	mib[3] = family;		/* only addresses of this family */
	mib[4] = NET_RT_DUMP;
	mib[5] = flags;			/* not looked at with NET_RT_DUMP */
	if (sysctl(mib, 6, NULL, lenp, NULL, 0) < 0)
		return(NULL);

	if ( (buf = malloc(*lenp)) == NULL)
		return(NULL);
	if (sysctl(mib, 6, buf, lenp, NULL, 0) < 0)
		return(NULL);

	return(buf);
}
/* end net_rt_dump */

char *
Net_rt_dump(int family, int flags, size_t *lenp)
{
	char	*ptr;

	if ( (ptr = net_rt_dump(family, flags, lenp)) == NULL)
		err_sys("net_rt_dump error");
	return(ptr);
}
