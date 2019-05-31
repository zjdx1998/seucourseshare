#include	"unp.h"

int
main(int argc, char **argv)
{
	char			*ptr, **pptr;
	struct netent	*nptr;

	while (--argc > 0) {
		ptr = *++argv;
		if ( (nptr = getnetbyname(ptr)) == NULL) {
			err_msg("getnetbyname error for net: %s: %s",
					ptr, hstrerror(h_errno));
			continue;
		}
		printf("official netname: %s\n", nptr->n_name);

		for (pptr = nptr->n_aliases; *pptr != NULL; pptr++)
			printf("	alias: %s\n", *pptr);

		switch (nptr->n_addrtype) {
		case AF_INET:
#ifdef	AF_INET6
		case AF_INET6:
#endif
			break;

		default:
			err_ret("unknown address type");
			break;
		}
	}
	exit(0);
}
