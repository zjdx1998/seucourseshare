#include	"unp.h"

ssize_t
recvfrom_flags(int fd, void *ptr, size_t nbytes, int *flagsp,
			   SA *sa, socklen_t *salenptr, void *dstaddrp)
{
	struct msghdr	msg;
	struct iovec	iov[1];
	ssize_t			n;

#ifdef	HAVE_MSGHDR_MSG_CONTROL
#define	CONTROL_LEN	(sizeof(struct cmsghdr) + sizeof(struct in_addr))
	char		control[CONTROL_LEN];

	msg.msg_control = control;
	msg.msg_controllen = sizeof(control);
	msg.msg_flags = 0;
#else
	bzero(&msg, sizeof(msg));	/* make certain msg_accrightslen = 0 */
#endif

	msg.msg_name = sa;
	msg.msg_namelen = *salenptr;
	iov[0].iov_base = ptr;
	iov[0].iov_len = nbytes;
	msg.msg_iov = iov;
	msg.msg_iovlen = 1;

	if ( (n = recvmsg(fd, &msg, *flagsp)) < 0)
		return(n);

	*salenptr = msg.msg_namelen;	/* pass back results */
#ifdef	HAVE_MSGHDR_MSG_CONTROL
	*flagsp = msg.msg_flags;		/* pass back results */

	/*
	 * We need someway to indicate to the caller that nothing was
	 * returned through dstaddrp (since all 32-bit values are valid
	 * IP destination addresses).  To avoid having to add yet another
	 * pointer argument, we return MSG_CTRUNC (which unp.h always
	 * defines, even if the implementation does not).
	 */

#ifdef	IP_RECVDSTADDR
	if (dstaddrp && msg.msg_controllen > 0 &&
		(msg.msg_flags & MSG_CTRUNC) == 0) {
		struct cmsghdr	*cmptr = CMSG_FIRSTHDR(&msg);

		if (cmptr->cmsg_len != CONTROL_LEN)
			err_quit("control length = %d", cmptr->cmsg_len);
		if (cmptr->cmsg_level != IPPROTO_IP)
			err_quit("control level != IPPROTO_IP");
		if (cmptr->cmsg_type != IP_RECVDSTADDR)
			err_quit("control type != IP_RECVDSTADDR");
		memcpy(dstaddrp, CMSG_DATA(cmptr), sizeof(struct in_addr));
	} else
#endif
		*flagsp |= MSG_CTRUNC;		/* dest IP addr not returned */
#else
	*flagsp |= MSG_CTRUNC;			/* dest IP addr not returned */
#endif	/* HAVE_MSGHDR_MSG_CONTROL */

	return(n);
}

ssize_t
Recvfrom_flags(int fd, void *ptr, size_t nbytes, int *flagsp,
			   SA *sa, socklen_t *salenptr, void *dstaddrp)
{
	ssize_t		n;

	n = recvfrom_flags(fd, ptr, nbytes, flagsp, sa, salenptr, dstaddrp);
	if (n < 0)
		err_quit("recvfrom_flags error");

	return(n);
}
