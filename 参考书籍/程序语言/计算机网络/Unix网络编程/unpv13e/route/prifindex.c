#include	"unpifi.h"

int
main(int argc, char **argv)
{
	if (argc != 2)
		err_quit("usage: prifname <interface-name>");

	printf("interface index = %d\n", If_nametoindex(argv[1]));
	exit(0);
}
