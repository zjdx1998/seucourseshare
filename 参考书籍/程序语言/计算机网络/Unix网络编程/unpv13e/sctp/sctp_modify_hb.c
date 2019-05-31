#include	"unp.h"

int heartbeat_action(int sock_fd, struct sockaddr *sa, socklen_t salen,
			  u_int value)
{
	struct sctp_paddrparams sp;
	int siz;

	bzero(&sp,sizeof(sp));
	sp.spp_hbinterval = value;
	memcpy((caddr_t)&sp.spp_address,sa,salen);
	Setsockopt(sock_fd,IPPROTO_SCTP,
		   SCTP_PEER_ADDR_PARAMS, &sp, sizeof(sp));
	return(0);
}
