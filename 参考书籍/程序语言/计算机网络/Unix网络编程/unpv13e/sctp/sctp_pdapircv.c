#include	"unp.h"

static uint8_t *sctp_pdapi_readbuf=NULL;
static int sctp_pdapi_rdbuf_sz=0;


uint8_t *
pdapi_recvmsg(int sock_fd,
	      int *rdlen,
	      SA *from,
	      int *from_len,
	      struct sctp_sndrcvinfo *sri,
	      int *msg_flags)
{
	int rdsz,left,at_in_buf;
	int frmlen=0;

	if (sctp_pdapi_readbuf == NULL) {
		sctp_pdapi_readbuf = (uint8_t *)Malloc(SCTP_PDAPI_INCR_SZ);
		sctp_pdapi_rdbuf_sz = SCTP_PDAPI_INCR_SZ;
	}
	at_in_buf = Sctp_recvmsg(sock_fd, sctp_pdapi_readbuf, sctp_pdapi_rdbuf_sz,
				 from, from_len,
				 sri,msg_flags);
	if(at_in_buf < 1){
		*rdlen = at_in_buf;
		return(NULL);
	}
	while((*msg_flags & MSG_EOR) == 0) {
		left = sctp_pdapi_rdbuf_sz - at_in_buf;
		if(left < SCTP_PDAPI_NEED_MORE_THRESHOLD) {
			sctp_pdapi_readbuf = realloc(sctp_pdapi_readbuf, sctp_pdapi_rdbuf_sz+SCTP_PDAPI_INCR_SZ);
			if(sctp_pdapi_readbuf == NULL) {
				err_quit("sctp_pdapi ran out of memory");
			}
			sctp_pdapi_rdbuf_sz += SCTP_PDAPI_INCR_SZ;
			left = sctp_pdapi_rdbuf_sz - at_in_buf;
		}
		rdsz = Sctp_recvmsg(sock_fd, &sctp_pdapi_readbuf[at_in_buf], 
			     left, NULL, &frmlen, NULL, msg_flags);
		at_in_buf += rdsz;
	}
	*rdlen = at_in_buf;
	return(sctp_pdapi_readbuf);
}
