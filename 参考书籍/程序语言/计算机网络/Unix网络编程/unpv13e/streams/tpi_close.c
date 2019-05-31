#include	"tpi_daytime.h"

void
tpi_close(int fd)
{
	struct T_ordrel_req	ordrel_req;
	struct strbuf		ctlbuf;

	ordrel_req.PRIM_type = T_ORDREL_REQ;

	ctlbuf.len = sizeof(struct T_ordrel_req);
	ctlbuf.buf = (char *) &ordrel_req;
	Putmsg(fd, &ctlbuf, NULL, 0);

	Close(fd);
}
