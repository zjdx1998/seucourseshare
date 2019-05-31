#include	"tpi_daytime.h"

ssize_t
tpi_read(int fd, void *buf, size_t len)
{
	struct strbuf		ctlbuf;
	struct strbuf		datbuf;
	union T_primitives	rcvbuf;
	int					flags;

	ctlbuf.maxlen = sizeof(union T_primitives);
	ctlbuf.buf = (char *) &rcvbuf;

	datbuf.maxlen = len;
	datbuf.buf = buf;
	datbuf.len = 0;

	flags = 0;
	Getmsg(fd, &ctlbuf, &datbuf, &flags);

	if (ctlbuf.len >= (int) sizeof(long)) {
		if (rcvbuf.type == T_DATA_IND)
			return(datbuf.len);
		else if (rcvbuf.type == T_ORDREL_IND)
			return(0);
		else
			err_quit("tpi_read: unexpected type %d", rcvbuf.type);
	} else if (ctlbuf.len == -1)
		return(datbuf.len);
	else
		err_quit("tpi_read: bad length from getmsg");
}
