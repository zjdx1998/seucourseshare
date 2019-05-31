/* include get_ifi_info1 */
#include	"unpifi.h"

struct ifi_info *
get_ifi_info(int family, int doaliases)
{
	struct ifi_info		*ifi, *ifihead, **ifipnext;
	int					sockfd, len, lastlen, flags, myflags, idx = 0, hlen = 0;
	char				*ptr, *buf, lastname[IFNAMSIZ], *cptr, *haddr, *sdlname;
	struct ifconf		ifc;
	struct ifreq		*ifr, ifrcopy;
	struct sockaddr_in	*sinptr;
	struct sockaddr_in6	*sin6ptr;

	sockfd = Socket(AF_INET, SOCK_DGRAM, 0);

	lastlen = 0;
	len = 100 * sizeof(struct ifreq);	/* initial buffer size guess */
	for ( ; ; ) {
		buf = Malloc(len);
		ifc.ifc_len = len;
		ifc.ifc_buf = buf;
		if (ioctl(sockfd, SIOCGIFCONF, &ifc) < 0) {
			if (errno != EINVAL || lastlen != 0)
				err_sys("ioctl error");
		} else {
			if (ifc.ifc_len == lastlen)
				break;		/* success, len has not changed */
			lastlen = ifc.ifc_len;
		}
		len += 10 * sizeof(struct ifreq);	/* increment */
		free(buf);
	}
	ifihead = NULL;
	ifipnext = &ifihead;
	lastname[0] = 0;
	sdlname = NULL;
/* end get_ifi_info1 */

/* include get_ifi_info2 */
	for (ptr = buf; ptr < buf + ifc.ifc_len; ) {
		ifr = (struct ifreq *) ptr;

#ifdef	HAVE_SOCKADDR_SA_LEN
		len = max(sizeof(struct sockaddr), ifr->ifr_addr.sa_len);
#else
		switch (ifr->ifr_addr.sa_family) {
#ifdef	IPV6
		case AF_INET6:	
			len = sizeof(struct sockaddr_in6);
			break;
#endif
		case AF_INET:	
		default:	
			len = sizeof(struct sockaddr);
			break;
		}
#endif	/* HAVE_SOCKADDR_SA_LEN */
		ptr += sizeof(ifr->ifr_name) + len;	/* for next one in buffer */

#ifdef	HAVE_SOCKADDR_DL_STRUCT
		/* assumes that AF_LINK precedes AF_INET or AF_INET6 */
		if (ifr->ifr_addr.sa_family == AF_LINK) {
			struct sockaddr_dl *sdl = (struct sockaddr_dl *)&ifr->ifr_addr;
			sdlname = ifr->ifr_name;
			idx = sdl->sdl_index;
			haddr = sdl->sdl_data + sdl->sdl_nlen;
			hlen = sdl->sdl_alen;
		}
#endif

		if (ifr->ifr_addr.sa_family != family)
			continue;	/* ignore if not desired address family */

		myflags = 0;
		if ( (cptr = strchr(ifr->ifr_name, ':')) != NULL)
			*cptr = 0;		/* replace colon with null */
		if (strncmp(lastname, ifr->ifr_name, IFNAMSIZ) == 0) {
			if (doaliases == 0)
				continue;	/* already processed this interface */
			myflags = IFI_ALIAS;
		}
		memcpy(lastname, ifr->ifr_name, IFNAMSIZ);

		ifrcopy = *ifr;
		Ioctl(sockfd, SIOCGIFFLAGS, &ifrcopy);
		flags = ifrcopy.ifr_flags;
		if ((flags & IFF_UP) == 0)
			continue;	/* ignore if interface not up */
/* end get_ifi_info2 */

/* include get_ifi_info3 */
		ifi = Calloc(1, sizeof(struct ifi_info));
		*ifipnext = ifi;			/* prev points to this new one */
		ifipnext = &ifi->ifi_next;	/* pointer to next one goes here */

		ifi->ifi_flags = flags;		/* IFF_xxx values */
		ifi->ifi_myflags = myflags;	/* IFI_xxx values */
#if defined(SIOCGIFMTU) && defined(HAVE_STRUCT_IFREQ_IFR_MTU)
		Ioctl(sockfd, SIOCGIFMTU, &ifrcopy);
		ifi->ifi_mtu = ifrcopy.ifr_mtu;
#else
		ifi->ifi_mtu = 0;
#endif
		memcpy(ifi->ifi_name, ifr->ifr_name, IFI_NAME);
		ifi->ifi_name[IFI_NAME-1] = '\0';
		/* If the sockaddr_dl is from a different interface, ignore it */
		if (sdlname == NULL || strcmp(sdlname, ifr->ifr_name) != 0)
			idx = hlen = 0;
		ifi->ifi_index = idx;
		ifi->ifi_hlen = hlen;
		if (ifi->ifi_hlen > IFI_HADDR)
			ifi->ifi_hlen = IFI_HADDR;
		if (hlen)
			memcpy(ifi->ifi_haddr, haddr, ifi->ifi_hlen);
/* end get_ifi_info3 */
/* include get_ifi_info4 */
		switch (ifr->ifr_addr.sa_family) {
		case AF_INET:
			sinptr = (struct sockaddr_in *) &ifr->ifr_addr;
			ifi->ifi_addr = Calloc(1, sizeof(struct sockaddr_in));
			memcpy(ifi->ifi_addr, sinptr, sizeof(struct sockaddr_in));

#ifdef	SIOCGIFBRDADDR
			if (flags & IFF_BROADCAST) {
				Ioctl(sockfd, SIOCGIFBRDADDR, &ifrcopy);
				sinptr = (struct sockaddr_in *) &ifrcopy.ifr_broadaddr;
				ifi->ifi_brdaddr = Calloc(1, sizeof(struct sockaddr_in));
				memcpy(ifi->ifi_brdaddr, sinptr, sizeof(struct sockaddr_in));
			}
#endif

#ifdef	SIOCGIFDSTADDR
			if (flags & IFF_POINTOPOINT) {
				Ioctl(sockfd, SIOCGIFDSTADDR, &ifrcopy);
				sinptr = (struct sockaddr_in *) &ifrcopy.ifr_dstaddr;
				ifi->ifi_dstaddr = Calloc(1, sizeof(struct sockaddr_in));
				memcpy(ifi->ifi_dstaddr, sinptr, sizeof(struct sockaddr_in));
			}
#endif
			break;

		case AF_INET6:
			sin6ptr = (struct sockaddr_in6 *) &ifr->ifr_addr;
			ifi->ifi_addr = Calloc(1, sizeof(struct sockaddr_in6));
			memcpy(ifi->ifi_addr, sin6ptr, sizeof(struct sockaddr_in6));

#ifdef	SIOCGIFDSTADDR
			if (flags & IFF_POINTOPOINT) {
				Ioctl(sockfd, SIOCGIFDSTADDR, &ifrcopy);
				sin6ptr = (struct sockaddr_in6 *) &ifrcopy.ifr_dstaddr;
				ifi->ifi_dstaddr = Calloc(1, sizeof(struct sockaddr_in6));
				memcpy(ifi->ifi_dstaddr, sin6ptr, sizeof(struct sockaddr_in6));
			}
#endif
			break;

		default:
			break;
		}
	}
	free(buf);
	return(ifihead);	/* pointer to first structure in linked list */
}
/* end get_ifi_info4 */

/* include free_ifi_info */
void
free_ifi_info(struct ifi_info *ifihead)
{
	struct ifi_info	*ifi, *ifinext;

	for (ifi = ifihead; ifi != NULL; ifi = ifinext) {
		if (ifi->ifi_addr != NULL)
			free(ifi->ifi_addr);
		if (ifi->ifi_brdaddr != NULL)
			free(ifi->ifi_brdaddr);
		if (ifi->ifi_dstaddr != NULL)
			free(ifi->ifi_dstaddr);
		ifinext = ifi->ifi_next;	/* can't fetch ifi_next after free() */
		free(ifi);					/* the ifi_info{} itself */
	}
}
/* end free_ifi_info */

struct ifi_info *
Get_ifi_info(int family, int doaliases)
{
	struct ifi_info	*ifi;

	if ( (ifi = get_ifi_info(family, doaliases)) == NULL)
		err_quit("get_ifi_info error");
	return(ifi);
}
