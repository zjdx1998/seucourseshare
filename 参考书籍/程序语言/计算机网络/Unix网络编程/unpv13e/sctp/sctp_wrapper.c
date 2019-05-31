#include	"unp.h"


int
Sctp_recvmsg(int s, void *msg, size_t len,
	     struct sockaddr *from, socklen_t *fromlen,
	     struct sctp_sndrcvinfo *sinfo,
	     int *msg_flags)
{
	int ret;
	ret = sctp_recvmsg(s,msg,len,from,fromlen,sinfo,msg_flags);
	if(ret < 0){
		err_sys("sctp_recvmsg error");
	}
	return(ret);
}


int
Sctp_sendmsg (int s, void *data, size_t len, struct sockaddr *to,
	      socklen_t tolen, uint32_t ppid, uint32_t flags,
	      uint16_t stream_no, uint32_t timetolive, uint32_t context)
{
	int ret;
	ret = sctp_sendmsg(s,data,len,to,tolen,ppid,flags,stream_no,
			  timetolive,context);
	if(ret < 0){
		err_sys("sctp_sendmsg error");
	}
	return(ret);
}

int
Sctp_bindx(int sock_fd,struct sockaddr_storage *at,int num,int op)
{
	int ret;
	ret = sctp_bindx(sock_fd,at,num,op);
	if(ret < 0){
		err_sys("sctp_bindx error");
	}
	return(ret);
}
