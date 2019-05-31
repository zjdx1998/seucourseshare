#include	"unproute.h"

/*
 * Round up 'a' to next multiple of 'size', which must be a power of 2
 */
#define ROUNDUP(a, size) (((a) & ((size)-1)) ? (1 + ((a) | ((size)-1))) : (a))

/*
 * Step to next socket address structure;
 * if sa_len is 0, assume it is sizeof(u_long).
 */
#define NEXT_SA(ap)	ap = (SA *) \
	((caddr_t) ap + (ap->sa_len ? ROUNDUP(ap->sa_len, sizeof (u_long)) : \
									sizeof(u_long)))

void
get_rtaddrs(int addrs, SA *sa, SA **rti_info)
{
	int		i;

	for (i = 0; i < RTAX_MAX; i++) {
		if (addrs & (1 << i)) {
			rti_info[i] = sa;
			NEXT_SA(sa);
		} else
			rti_info[i] = NULL;
	}
}
