#include	"unp.h"

int
main(int argc, char **argv)
{
	int sock_fd,msg_flags,connfd,childpid;
	sctp_assoc_t assoc;
	char readbuf[BUFFSIZE];
	struct sockaddr_in servaddr, cliaddr;
	struct sctp_sndrcvinfo sri;
	struct sctp_event_subscribe evnts;
	socklen_t len;
	size_t rd_sz;

        sock_fd = Socket(AF_INET, SOCK_SEQPACKET, IPPROTO_SCTP);
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(SERV_PORT);

	Bind(sock_fd, (SA *) &servaddr, sizeof(servaddr));
	
	bzero(&evnts, sizeof(evnts));
	evnts.sctp_data_io_event = 1;
	Setsockopt(sock_fd, IPPROTO_SCTP, SCTP_EVENTS,
		   &evnts, sizeof(evnts));

	Listen(sock_fd, LISTENQ);
/* include mod_servfork */
	for ( ; ; ) {
		len = sizeof(struct sockaddr_in);
		rd_sz = Sctp_recvmsg(sock_fd, readbuf, sizeof(readbuf),
			     (SA *)&cliaddr, &len,
			     &sri,&msg_flags);
		Sctp_sendmsg(sock_fd, readbuf, rd_sz, 
			     (SA *)&cliaddr, len,
			     sri.sinfo_ppid,
			     sri.sinfo_flags,
			     sri.sinfo_stream,
			     0, 0);
		assoc = sctp_address_to_associd(sock_fd,(SA *)&cliaddr,len);
		if((int)assoc == 0){
			err_ret("Can't get association id");
			continue;
		} 
		connfd = sctp_peeloff(sock_fd,assoc);
		if(connfd == -1){
			err_ret("sctp_peeloff fails");
			continue;
		}
		if((childpid = fork()) == 0) {
			Close(sock_fd);
			str_echo(connfd);
			exit(0);
		} else {
			Close(connfd);
		}
	}
/* end mod_servfork */
}
