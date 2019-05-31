#include	"unpifi.h"

int
main(int argc, char **argv)
{
	char	name[16];

	if (argc != 2)
		err_quit("usage: prifname <interface-index>");

	printf("interface name = %s\n", If_indextoname(atoi(argv[1]), name));
	exit(0);
}
