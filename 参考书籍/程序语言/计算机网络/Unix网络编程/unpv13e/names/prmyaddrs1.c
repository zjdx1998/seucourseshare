#include	"unp.h"

char	**my_addrs(int *);

int
main(int argc, char **argv)
{
	int		addrtype;
	char	**pptr, buf[INET6_ADDRSTRLEN];

	if ( (pptr = my_addrs(&addrtype)) == NULL)
		err_quit("my_addrs error");

	for ( ; *pptr != NULL; pptr++)
		printf("\taddress: %s\n",
			   Inet_ntop(addrtype, *pptr, buf, sizeof(buf)));

	exit(0);
}
