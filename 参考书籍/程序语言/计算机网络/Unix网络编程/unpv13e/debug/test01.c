#include	"unpxti.h"

int
main(int argc, char **argv)
{
	int		tfd;

	if (argc != 3)
		err_quit("usage: test01 <hostname/IPaddress> <service/port#>");

	tfd = Tcp_connect(argv[1], argv[2]);

	t_snd(tfd, "", 1, T_EXPEDITED);

	exit(0);
}
