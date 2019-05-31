#include	"tpi_daytime.h"

void
tpi_connect(int fd, const void *addr, size_t addrlen)
{
	struct {
	  struct T_conn_req	msg_hdr;
	  char				addr[128];
	} conn_req;
	struct {
	  struct T_conn_con	msg_hdr;
	  char				addr[128];
	} conn_con;
	struct strbuf		ctlbuf;
	union T_primitives	rcvbuf;
	struct T_error_ack	*error_ack;
	struct T_discon_ind	*discon_ind;
	int					flags;

	conn_req.msg_hdr.PRIM_type = T_CONN_REQ;
	conn_req.msg_hdr.DEST_length = addrlen;
	conn_req.msg_hdr.DEST_offset = sizeof(struct T_conn_req);
	conn_req.msg_hdr.OPT_length = 0;
	conn_req.msg_hdr.OPT_offset = 0;
	memcpy(conn_req.addr, addr, addrlen);	/* sockaddr_in{} */

	ctlbuf.len = sizeof(struct T_conn_req) + addrlen;
	ctlbuf.buf = (char *) &conn_req;
	Putmsg(fd, &ctlbuf, NULL, 0);

	ctlbuf.maxlen = sizeof(union T_primitives);
	ctlbuf.len = 0;
	ctlbuf.buf = (char *) &rcvbuf;
	flags = RS_HIPRI;
	Getmsg(fd, &ctlbuf, NULL, &flags);

/* *INDENT-OFF* */
	if (ctlbuf.len < (int) sizeof(long))
		err_quit("tpi_connect: bad length from getmsg");
/* *INDENT-ON* */

	switch(rcvbuf.type) {
    case T_OK_ACK:
		break;

    case T_ERROR_ACK:
/* *INDENT-OFF* */
		if (ctlbuf.len < (int) sizeof(struct T_error_ack))
			err_quit("tpi_connect: bad length for T_ERROR_ACK");
		error_ack = (struct T_error_ack *) &rcvbuf;
		err_quit("tpi_connect: T_ERROR_ACK from conn (%d, %d)",
				 error_ack->TLI_error, error_ack->UNIX_error);
/* *INDENT-ON* */

    default:
		err_quit("tpi_connect: unexpected message type: %d", rcvbuf.type);
	}

	ctlbuf.maxlen = sizeof(conn_con);
	ctlbuf.len = 0;
	ctlbuf.buf = (char *) &conn_con;
	flags = 0;
	Getmsg(fd, &ctlbuf, NULL, &flags);

/* *INDENT-OFF* */
	if (ctlbuf.len < (int) sizeof(long))
		err_quit("tpi_connect2: bad length from getmsg");
/* *INDENT-ON* */

	switch(conn_con.msg_hdr.PRIM_type) {
    case T_CONN_CON:
		break;

    case T_DISCON_IND:
/* *INDENT-OFF* */
		if (ctlbuf.len < (int) sizeof(struct T_discon_ind))
			err_quit("tpi_connect2: bad length for T_DISCON_IND");
		discon_ind = (struct T_discon_ind *) &conn_con.msg_hdr;
		err_quit("tpi_connect2: T_DISCON_IND from conn (%d)",
				 discon_ind->DISCON_reason);
/* *INDENT-ON* */

    default:
		err_quit("tpi_connect2: unexpected message type: %d",
				 conn_con.msg_hdr.PRIM_type);
	}
}
