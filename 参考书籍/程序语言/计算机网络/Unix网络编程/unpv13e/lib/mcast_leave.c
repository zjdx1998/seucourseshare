#include	"unp.h"

int
mcast_leave(int sockfd, const SA *grp, socklen_t grplen)
{
#ifdef MCAST_JOIN_GROUP
	struct group_req req;
	req.gr_interface = 0;
	if (grplen > sizeof(req.gr_group)) {
		errno = EINVAL;
		return -1;
	}
	memcpy(&req.gr_group, grp, grplen);
	return (setsockopt(sockfd, family_to_level(grp->sa_family),
			MCAST_LEAVE_GROUP, &req, sizeof(req)));
#else
	switch (grp->sa_family) {
	case AF_INET: {
		struct ip_mreq		mreq;

		memcpy(&mreq.imr_multiaddr,
			   &((const struct sockaddr_in *) grp)->sin_addr,
			   sizeof(struct in_addr));
		mreq.imr_interface.s_addr = htonl(INADDR_ANY);
		return(setsockopt(sockfd, IPPROTO_IP, IP_DROP_MEMBERSHIP,
						  &mreq, sizeof(mreq)));
	}

#ifdef	IPV6
#ifndef	IPV6_LEAVE_GROUP	/* APIv0 compatibility */
#define	IPV6_LEAVE_GROUP	IPV6_DROP_MEMBERSHIP
#endif
	case AF_INET6: {
		struct ipv6_mreq	mreq6;

		memcpy(&mreq6.ipv6mr_multiaddr,
			   &((const struct sockaddr_in6 *) grp)->sin6_addr,
			   sizeof(struct in6_addr));
		mreq6.ipv6mr_interface = 0;
		return(setsockopt(sockfd, IPPROTO_IPV6, IPV6_LEAVE_GROUP,
						  &mreq6, sizeof(mreq6)));
	}
#endif

	default:
		errno = EAFNOSUPPORT;
		return(-1);
	}
#endif
}

void
Mcast_leave(int sockfd, const SA *grp, socklen_t grplen)
{
	if (mcast_leave(sockfd, grp, grplen) < 0)
		err_sys("mcast_leave error");
}

int
mcast_leave_source_group(int sockfd, const SA *src, socklen_t srclen,
						const SA *grp, socklen_t grplen)
{
#ifdef MCAST_LEAVE_SOURCE_GROUP
	struct group_source_req req;
	req.gsr_interface = 0;
	if (grplen > sizeof(req.gsr_group) || srclen > sizeof(req.gsr_source)) {
		errno = EINVAL;
		return -1;
	}
	memcpy(&req.gsr_group, grp, grplen);
	memcpy(&req.gsr_source, src, srclen);
	return (setsockopt(sockfd, family_to_level(grp->sa_family),
			MCAST_LEAVE_SOURCE_GROUP, &req, sizeof(req)));
#else
	switch (grp->sa_family) {
#ifdef IP_DROP_SOURCE_MEMBERSHIP
	case AF_INET: {
		struct ip_mreq_source	mreq;

		memcpy(&mreq.imr_multiaddr,
			   &((struct sockaddr_in *) grp)->sin_addr,
			   sizeof(struct in_addr));
		memcpy(&mreq.imr_sourceaddr,
			   &((struct sockaddr_in *) src)->sin_addr,
			   sizeof(struct in_addr));
		mreq.imr_interface.s_addr = htonl(INADDR_ANY);

		return(setsockopt(sockfd, IPPROTO_IP, IP_DROP_SOURCE_MEMBERSHIP,
						  &mreq, sizeof(mreq)));
	}
#endif

#ifdef	IPV6
	case AF_INET6: /* IPv6 source-specific API is MCAST_LEAVE_SOURCE_GROUP */
#endif
	default:
		errno = EAFNOSUPPORT;
		return(-1);
	}
#endif
}

void
Mcast_leave_source_group(int sockfd, const SA *src, socklen_t srclen,
						const SA *grp, socklen_t grplen)
{
	if (mcast_leave_source_group(sockfd, src, srclen, grp, grplen) < 0)
		err_sys("mcast_leave_source_group error");
}
