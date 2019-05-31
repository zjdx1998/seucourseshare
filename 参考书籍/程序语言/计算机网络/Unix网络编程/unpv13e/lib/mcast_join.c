/* include mcast_join1 */
#include	"unp.h"
#include	<net/if.h>

int
mcast_join(int sockfd, const SA *grp, socklen_t grplen,
		   const char *ifname, u_int ifindex)
{
#ifdef MCAST_JOIN_GROUP
	struct group_req req;
	if (ifindex > 0) {
		req.gr_interface = ifindex;
	} else if (ifname != NULL) {
		if ( (req.gr_interface = if_nametoindex(ifname)) == 0) {
			errno = ENXIO;	/* i/f name not found */
			return(-1);
		}
	} else
		req.gr_interface = 0;
	if (grplen > sizeof(req.gr_group)) {
		errno = EINVAL;
		return -1;
	}
	memcpy(&req.gr_group, grp, grplen);
	return (setsockopt(sockfd, family_to_level(grp->sa_family),
			MCAST_JOIN_GROUP, &req, sizeof(req)));
#else
/* end mcast_join1 */

/* include mcast_join2 */
	switch (grp->sa_family) {
	case AF_INET: {
		struct ip_mreq		mreq;
		struct ifreq		ifreq;

		memcpy(&mreq.imr_multiaddr,
			   &((const struct sockaddr_in *) grp)->sin_addr,
			   sizeof(struct in_addr));

		if (ifindex > 0) {
			if (if_indextoname(ifindex, ifreq.ifr_name) == NULL) {
				errno = ENXIO;	/* i/f index not found */
				return(-1);
			}
			goto doioctl;
		} else if (ifname != NULL) {
			strncpy(ifreq.ifr_name, ifname, IFNAMSIZ);
doioctl:
			if (ioctl(sockfd, SIOCGIFADDR, &ifreq) < 0)
				return(-1);
			memcpy(&mreq.imr_interface,
				   &((struct sockaddr_in *) &ifreq.ifr_addr)->sin_addr,
				   sizeof(struct in_addr));
		} else
			mreq.imr_interface.s_addr = htonl(INADDR_ANY);

		return(setsockopt(sockfd, IPPROTO_IP, IP_ADD_MEMBERSHIP,
						  &mreq, sizeof(mreq)));
	}
/* end mcast_join2 */

/* include mcast_join3 */
#ifdef	IPV6
#ifndef	IPV6_JOIN_GROUP		/* APIv0 compatibility */
#define	IPV6_JOIN_GROUP		IPV6_ADD_MEMBERSHIP
#endif
	case AF_INET6: {
		struct ipv6_mreq	mreq6;

		memcpy(&mreq6.ipv6mr_multiaddr,
			   &((const struct sockaddr_in6 *) grp)->sin6_addr,
			   sizeof(struct in6_addr));

		if (ifindex > 0) {
			mreq6.ipv6mr_interface = ifindex;
		} else if (ifname != NULL) {
			if ( (mreq6.ipv6mr_interface = if_nametoindex(ifname)) == 0) {
				errno = ENXIO;	/* i/f name not found */
				return(-1);
			}
		} else
			mreq6.ipv6mr_interface = 0;

		return(setsockopt(sockfd, IPPROTO_IPV6, IPV6_JOIN_GROUP,
						  &mreq6, sizeof(mreq6)));
	}
#endif

	default:
		errno = EAFNOSUPPORT;
		return(-1);
	}
#endif
}
/* end mcast_join3 */

void
Mcast_join(int sockfd, const SA *grp, socklen_t grplen,
		   const char *ifname, u_int ifindex)
{
	if (mcast_join(sockfd, grp, grplen, ifname, ifindex) < 0)
		err_sys("mcast_join error");
}

int
mcast_join_source_group(int sockfd, const SA *src, socklen_t srclen,
						const SA *grp, socklen_t grplen,
						const char *ifname, u_int ifindex)
{
#ifdef MCAST_JOIN_SOURCE_GROUP
	struct group_source_req req;
	if (ifindex > 0) {
		req.gsr_interface = ifindex;
	} else if (ifname != NULL) {
		if ( (req.gsr_interface = if_nametoindex(ifname)) == 0) {
			errno = ENXIO;	/* i/f name not found */
			return(-1);
		}
	} else
		req.gsr_interface = 0;
	if (grplen > sizeof(req.gsr_group) || srclen > sizeof(req.gsr_source)) {
		errno = EINVAL;
		return -1;
	}
	memcpy(&req.gsr_group, grp, grplen);
	memcpy(&req.gsr_source, src, srclen);
	return (setsockopt(sockfd, family_to_level(grp->sa_family),
			MCAST_JOIN_SOURCE_GROUP, &req, sizeof(req)));
#else
	switch (grp->sa_family) {
#ifdef IP_ADD_SOURCE_MEMBERSHIP
	case AF_INET: {
		struct ip_mreq_source	mreq;
		struct ifreq			ifreq;

		memcpy(&mreq.imr_multiaddr,
			   &((struct sockaddr_in *) grp)->sin_addr,
			   sizeof(struct in_addr));
		memcpy(&mreq.imr_sourceaddr,
			   &((struct sockaddr_in *) src)->sin_addr,
			   sizeof(struct in_addr));

		if (ifindex > 0) {
			if (if_indextoname(ifindex, ifreq.ifr_name) == NULL) {
				errno = ENXIO;	/* i/f index not found */
				return(-1);
			}
			goto doioctl;
		} else if (ifname != NULL) {
			strncpy(ifreq.ifr_name, ifname, IFNAMSIZ);
doioctl:
			if (ioctl(sockfd, SIOCGIFADDR, &ifreq) < 0)
				return(-1);
			memcpy(&mreq.imr_interface,
				   &((struct sockaddr_in *) &ifreq.ifr_addr)->sin_addr,
				   sizeof(struct in_addr));
		} else
			mreq.imr_interface.s_addr = htonl(INADDR_ANY);

		return(setsockopt(sockfd, IPPROTO_IP, IP_ADD_SOURCE_MEMBERSHIP,
						  &mreq, sizeof(mreq)));
	}
#endif

#ifdef	IPV6
	case AF_INET6: /* IPv6 source-specific API is MCAST_JOIN_SOURCE_GROUP */
#endif
	default:
		errno = EAFNOSUPPORT;
		return(-1);
	}
#endif
}

void
Mcast_join_source_group(int sockfd, const SA *src, socklen_t srclen,
						const SA *grp, socklen_t grplen,
						const char *ifname, u_int ifindex)
{
	if (mcast_join_source_group(sockfd, src, srclen, grp, grplen,
								ifname, ifindex) < 0)
		err_sys("mcast_join_source_group error");
}

int
mcast_block_source(int sockfd, const SA *src, socklen_t srclen,
						const SA *grp, socklen_t grplen)
{
#ifdef MCAST_BLOCK_SOURCE
	struct group_source_req req;
	req.gsr_interface = 0;
	if (grplen > sizeof(req.gsr_group) || srclen > sizeof(req.gsr_source)) {
		errno = EINVAL;
		return -1;
	}
	memcpy(&req.gsr_group, grp, grplen);
	memcpy(&req.gsr_source, src, srclen);
	return (setsockopt(sockfd, family_to_level(grp->sa_family),
			MCAST_BLOCK_SOURCE, &req, sizeof(req)));
#else
	switch (grp->sa_family) {
#ifdef IP_BLOCK_SOURCE
	case AF_INET: {
		struct ip_mreq_source	mreq;

		memcpy(&mreq.imr_multiaddr,
			   &((struct sockaddr_in *) grp)->sin_addr,
			   sizeof(struct in_addr));
		memcpy(&mreq.imr_sourceaddr,
			   &((struct sockaddr_in *) src)->sin_addr,
			   sizeof(struct in_addr));
		mreq.imr_interface.s_addr = htonl(INADDR_ANY);

		return(setsockopt(sockfd, IPPROTO_IP, IP_BLOCK_SOURCE,
						  &mreq, sizeof(mreq)));
	}
#endif

#ifdef	IPV6
	case AF_INET6: /* IPv6 source-specific API is MCAST_BLOCK_SOURCE */
#endif
	default:
		errno = EAFNOSUPPORT;
		return(-1);
	}
#endif
}

void
Mcast_block_source(int sockfd, const SA *src, socklen_t srclen,
						const SA *grp, socklen_t grplen)
{
	if (mcast_block_source(sockfd, src, srclen, grp, grplen) < 0)
		err_sys("mcast_block_source error");
}

int
mcast_unblock_source(int sockfd, const SA *src, socklen_t srclen,
						const SA *grp, socklen_t grplen)
{
#ifdef MCAST_UNBLOCK_SOURCE
	struct group_source_req req;
	req.gsr_interface = 0;
	if (grplen > sizeof(req.gsr_group) || srclen > sizeof(req.gsr_source)) {
		errno = EINVAL;
		return -1;
	}
	memcpy(&req.gsr_group, grp, grplen);
	memcpy(&req.gsr_source, src, srclen);
	return (setsockopt(sockfd, family_to_level(grp->sa_family),
			MCAST_UNBLOCK_SOURCE, &req, sizeof(req)));
#else
	switch (grp->sa_family) {
#ifdef IP_UNBLOCK_SOURCE
	case AF_INET: {
		struct ip_mreq_source	mreq;

		memcpy(&mreq.imr_multiaddr,
			   &((struct sockaddr_in *) grp)->sin_addr,
			   sizeof(struct in_addr));
		memcpy(&mreq.imr_sourceaddr,
			   &((struct sockaddr_in *) src)->sin_addr,
			   sizeof(struct in_addr));
		mreq.imr_interface.s_addr = htonl(INADDR_ANY);

		return(setsockopt(sockfd, IPPROTO_IP, IP_UNBLOCK_SOURCE,
						  &mreq, sizeof(mreq)));
	}
#endif

#ifdef	IPV6
	case AF_INET6: /* IPv6 source-specific API is MCAST_UNBLOCK_SOURCE */
#endif
	default:
		errno = EAFNOSUPPORT;
		return(-1);
	}
#endif
}

void
Mcast_unblock_source(int sockfd, const SA *src, socklen_t srclen,
						const SA *grp, socklen_t grplen)
{
	if (mcast_unblock_source(sockfd, src, srclen, grp, grplen) < 0)
		err_sys("mcast_unblock_source error");
}
