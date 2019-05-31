/* include if_nametoindex */
#include	"unpifi.h"
#include	"unproute.h"

unsigned int
if_nametoindex(const char *name)
{
	unsigned int		idx, namelen;
	char				*buf, *next, *lim;
	size_t				len;
	struct if_msghdr	*ifm;
	struct sockaddr		*sa, *rti_info[RTAX_MAX];
	struct sockaddr_dl	*sdl;

	if ( (buf = net_rt_iflist(0, 0, &len)) == NULL)
		return(0);

	namelen = strlen(name);
	lim = buf + len;
	for (next = buf; next < lim; next += ifm->ifm_msglen) {
		ifm = (struct if_msghdr *) next;
		if (ifm->ifm_type == RTM_IFINFO) {
			sa = (struct sockaddr *) (ifm + 1);
			get_rtaddrs(ifm->ifm_addrs, sa, rti_info);
			if ( (sa = rti_info[RTAX_IFP]) != NULL) {
				if (sa->sa_family == AF_LINK) {
					sdl = (struct sockaddr_dl *) sa;
					if (sdl->sdl_nlen == namelen && strncmp(&sdl->sdl_data[0], name, sdl->sdl_nlen) == 0) {
						idx = sdl->sdl_index;	/* save before free() */
						free(buf);
						return(idx);
					}
				}
			}

		}
	}
	free(buf);
	return(0);		/* no match for name */
}
/* end if_nametoindex */

unsigned int
If_nametoindex(const char *name)
{
	int		idx;

	if ( (idx = if_nametoindex(name)) == 0)
		err_quit("if_nametoindex error for %s", name);
	return(idx);
}
