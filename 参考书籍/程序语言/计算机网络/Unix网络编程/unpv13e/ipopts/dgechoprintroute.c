#include	"unp.h"

void
dg_echo(int sockfd, SA *pcliaddr, socklen_t clilen)
{
	int				n;
	char			mesg[MAXLINE];
	int				on;
	char			control[MAXLINE];
	struct msghdr	msg;
	struct cmsghdr	*cmsg;
	struct iovec	iov[1];

	on = 1;
	Setsockopt(sockfd, IPPROTO_IPV6, IPV6_RECVRTHDR, &on, sizeof(on));

	bzero(&msg, sizeof(msg));
	iov[0].iov_base = mesg;
	msg.msg_name = pcliaddr;
	msg.msg_iov = iov;
	msg.msg_iovlen = 1;
	msg.msg_control = control;
	for ( ; ; ) {
		msg.msg_namelen = clilen;
		msg.msg_controllen = sizeof(control);
		iov[0].iov_len = MAXLINE;
		n = Recvmsg(sockfd, &msg, 0);

		for (cmsg = CMSG_FIRSTHDR(&msg); cmsg != NULL;
			 cmsg = CMSG_NXTHDR(&msg, cmsg)) {
			if (cmsg->cmsg_level == IPPROTO_IPV6 &&
				cmsg->cmsg_type == IPV6_RTHDR) {
				inet6_srcrt_print(CMSG_DATA(cmsg));
				Inet6_rth_reverse(CMSG_DATA(cmsg), CMSG_DATA(cmsg));
			}
		}

		iov[0].iov_len = n;
		Sendmsg(sockfd, &msg, 0);
	}
}
