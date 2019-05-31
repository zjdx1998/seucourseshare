#include	"unp.h"

int
main(int argc, char **argv)
{
	int		fd;
	fd_set	exset;

	if (argc != 3)
		err_quit("usage: test01 <hostname/IPaddress> <service/port#>");

	fd = Tcp_connect(argv[1], argv[2]);

	FD_ZERO(&exset);
	FD_SET(fd, &exset);
	select(fd+1, NULL, NULL, &exset, NULL);

	exit(0);
}
