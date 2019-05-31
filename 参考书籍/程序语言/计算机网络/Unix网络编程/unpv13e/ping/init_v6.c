#include "ping.h"

void
init_v6()
{
#ifdef IPV6
	int on = 1;

	if (verbose == 0) {
		/* install a filter that only passes ICMP6_ECHO_REPLY unless verbose */
		struct icmp6_filter myfilt;
		ICMP6_FILTER_SETBLOCKALL(&myfilt);
		ICMP6_FILTER_SETPASS(ICMP6_ECHO_REPLY, &myfilt);
		setsockopt(sockfd, IPPROTO_IPV6, ICMP6_FILTER, &myfilt, sizeof(myfilt));
		/* ignore error return; the filter is an optimization */
	}

	/* ignore error returned below; we just won't receive the hop limit */
#ifdef IPV6_RECVHOPLIMIT
	/* RFC 3542 */
	setsockopt(sockfd, IPPROTO_IPV6, IPV6_RECVHOPLIMIT, &on, sizeof(on));
#else
	/* RFC 2292 */
	setsockopt(sockfd, IPPROTO_IPV6, IPV6_HOPLIMIT, &on, sizeof(on));
#endif
#endif
}
