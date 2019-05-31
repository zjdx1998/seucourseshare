#include	"unp.h"

int
main(int argc, char **argv)
{
	int sock_fd,msg_flags;
	char readbuf[BUFFSIZE];
	struct sockaddr_in cliaddr;
	struct sctp_sndrcvinfo sri;
	struct sctp_event_subscribe evnts;
	socklen_t len;
	size_t rd_sz;
/* include mod_serv07 */
	if(argc < 2)
		err_quit("Error, use %s [list of addresses to bind]\n",
		       argv[0]);
        sock_fd = Socket(AF_INET6, SOCK_SEQPACKET, IPPROTO_SCTP);

	if(sctp_bind_arg_list(sock_fd, argv + 1, argc - 1))
		err_sys("Can't bind the address set");

	bzero(&evnts, sizeof(evnts));
	evnts.sctp_data_io_event = 1;
/* end mod_serv07 */
	evnts.sctp_association_event = 1;
	evnts.sctp_address_event = 1;
	evnts.sctp_send_failure_event = 1;
	evnts.sctp_peer_error_event = 1;
	evnts.sctp_shutdown_event = 1;
	evnts.sctp_partial_delivery_event = 1;
	evnts.sctp_adaption_layer_event = 1;
	Setsockopt(sock_fd, IPPROTO_SCTP, SCTP_EVENTS,
		   &evnts, sizeof(evnts));

	Listen(sock_fd, LISTENQ);

	for ( ; ; ) {
		len = sizeof(struct sockaddr_in);
		rd_sz = Sctp_recvmsg(sock_fd, readbuf, sizeof(readbuf),
			     (SA *)&cliaddr, &len,
			     &sri,&msg_flags);
		if(msg_flags & MSG_NOTIFICATION) {
			print_notification(readbuf);
			continue;
		}
		Sctp_sendmsg(sock_fd, readbuf, rd_sz, 
			     (SA *)&cliaddr, len,
			     sri.sinfo_ppid,
			     sri.sinfo_flags,
			     sri.sinfo_stream,
			     0, 0);
	}
	
}
