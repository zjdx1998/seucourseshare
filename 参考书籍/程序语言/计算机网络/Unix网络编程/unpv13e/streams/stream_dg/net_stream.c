#include <stdio.h>
#include <sys/types.h>
#include <sys/fcntl.h>
#include <sys/errno.h>

#include <sys/stream.h>
#include <sys/stropts.h>
#include <sys/tihdr.h>


extern int errno;

int
net_open (char *path, int oflags, void *addr, int addrlen)
{
	int fd;
	int flags;

	if ((fd = open (path, oflags)) < 0)
	{
		perror ("open");
		return (-1);
	}
	return (fd);
}

int
net_bind (int fd, void *addr, int addrlen)
{
	struct {
		struct T_bind_req msg_hdr;
		char addr[128];
	} bind_req;
	struct strbuf ctlbuf;
	union T_primitives rcvbuf;
	struct T_error_ack *error_ack;
	int flags;

	if (addr == NULL || addrlen == 0)
	{
		fprintf (stderr, "No address\n");
		return (-1);
	}

	bind_req.msg_hdr.PRIM_type = T_BIND_REQ;
	bind_req.msg_hdr.ADDR_length = addrlen;
	bind_req.msg_hdr.ADDR_offset = sizeof (struct T_bind_req);
	bind_req.msg_hdr.CONIND_number = 5;
	bcopy (addr, bind_req.addr, addrlen);

	ctlbuf.len = sizeof (struct T_bind_req) + addrlen;
	ctlbuf.buf = (char *) &bind_req;

	if (putmsg (fd, &ctlbuf, NULL, 0) < 0)
	{
		return (-1);
	}
	/*
	 * Wait for acknowledgement
	 */
	ctlbuf.maxlen = sizeof (union T_primitives);
	ctlbuf.len = 0;
	ctlbuf.buf = (char *) &rcvbuf;
	flags = RS_HIPRI;
	if (getmsg (fd, &ctlbuf, NULL, &flags) < 0)
	{
		perror ("getmsg");
		return (-1);
	}

	if (ctlbuf.len < sizeof (long))
	{
		fprintf (stderr, "Bad length from getmsg.\n");
		errno = EPROTO;
		return (-1);
	}
	switch (rcvbuf.type)
	{
	    case T_BIND_ACK:
		return (fd);
	    case T_ERROR_ACK:
		if (ctlbuf.len < sizeof (struct T_error_ack))
		{
			errno = EPROTO;
			return (-1);
		}
		error_ack = (struct T_error_ack *) &rcvbuf;
		fprintf (stderr, "Error ack from bind (%d %d %d)\n",
			error_ack->ERROR_prim,
			error_ack->TLI_error,
			error_ack->UNIX_error);
		errno = error_ack->UNIX_error;
		break;
	    default:
		fprintf (stderr, "No ack from bind?\n");
		errno = EPROTO;
		break;
	}
	return (-1);
}
int
net_send (int fd, char *buf, int len, char *to_addr, int addrlen)
{
	struct strbuf ctlbuf;
	struct strbuf databuf;
	struct {
		struct T_unitdata_req unitdata_req;
		char buf[128];
	} netdata;

	bcopy (to_addr, netdata.buf, addrlen);
	netdata.unitdata_req.PRIM_type = T_UNITDATA_REQ;
	netdata.unitdata_req.DEST_length = addrlen;
	netdata.unitdata_req.DEST_offset = sizeof (struct T_unitdata_req);
	/* 
	 * for now, presume no options.
	 */
	netdata.unitdata_req.OPT_length = 0;
	netdata.unitdata_req.OPT_offset = 0;

	ctlbuf.len = sizeof (struct T_unitdata_req) + addrlen;
	ctlbuf.buf = (char *) &netdata;

	databuf.len = len;
	databuf.buf = buf;

	if (putmsg (fd, &ctlbuf, &databuf, 0) < 0)
		return (0);
	free (netdata.buf);
	return (len);
}

int
net_recv (int fd, char *buf, int len, char *from_addr, int addrlen)
{
	struct strbuf ctlbuf;
	struct strbuf databuf;
	struct {
		struct T_unitdata_ind unitdata_ind;
		char buf[128];
	} netdata;
	char *c;
	int retval;
	int flags;

	ctlbuf.maxlen = sizeof (netdata);
	ctlbuf.buf = (char *) &netdata;

	databuf.maxlen = len;
	databuf.len = 0;
	databuf.buf = buf;

	flags = 0;

	if ((retval = getmsg (fd, &ctlbuf, &databuf, &flags)) < 0)
		return (-1);

	if (netdata.unitdata_ind.PRIM_type != T_UNITDATA_IND)
	{
		fprintf (stderr, "net_recv: Got %d\n", netdata.unitdata_ind.PRIM_type);
		errno = EPROTO;
		return (0);
	}
	if (retval)
	{
		errno = EIO;
		return (0);
	}
	/*
	 * Copy return address for the user
	 */
	if (netdata.unitdata_ind.SRC_length < addrlen)
		addrlen = netdata.unitdata_ind.SRC_length;

	c = (char *) &netdata;
	bcopy (&(c[netdata.unitdata_ind.SRC_offset]),
		from_addr,
		addrlen);

	return (databuf.len);
}
