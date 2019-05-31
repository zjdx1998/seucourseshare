#include	"unp.h"

#define	SCTP_MAXLINE	800

void
sctpstr_cli_echoall(FILE *fp, int sock_fd, struct sockaddr *to, socklen_t tolen)
{
	struct sockaddr_in peeraddr;
	struct sctp_sndrcvinfo sri;
	char sendline[SCTP_MAXLINE], recvline[SCTP_MAXLINE];
	socklen_t len;
	int rd_sz,i,strsz;
	int msg_flags;

	bzero(sendline,sizeof(sendline));
	bzero(&sri,sizeof(sri));
	while (fgets(sendline, SCTP_MAXLINE - 9, fp) != NULL) {
		strsz = strlen(sendline);
		if(sendline[strsz-1] == '\n') {
			sendline[strsz-1] = '\0';
			strsz--;
		}
		for(i=0;i<SERV_MAX_SCTP_STRM;i++) {
			snprintf(sendline + strsz, sizeof(sendline) - strsz,
				".msg.%d", i);
			Sctp_sendmsg(sock_fd, sendline, sizeof(sendline), 
				     to, tolen, 
				     0, 0,
				     i,
				     0, 0);
		}
		for(i=0;i<SERV_MAX_SCTP_STRM;i++) {
			len = sizeof(peeraddr);
			rd_sz = Sctp_recvmsg(sock_fd, recvline, sizeof(recvline),
				     (SA *)&peeraddr, &len,
				     &sri,&msg_flags);
			printf("From str:%d seq:%d (assoc:0x%x):",
				sri.sinfo_stream,sri.sinfo_ssn,
				(u_int)sri.sinfo_assoc_id);
			printf("%.*s\n",rd_sz,recvline);
		}
	}
}
