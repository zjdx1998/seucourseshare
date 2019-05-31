#include	"unp.h"

void	pr_ipv4(char **);

int
main(int argc, char **argv)
{
	char			*ptr, **pptr;
	struct hostent	*hptr;

	while (--argc > 0) {
		ptr = *++argv;
		if ( (hptr = gethostbyname(ptr)) == NULL) {
			err_msg("gethostbyname error for host: %s: %s",
					ptr, hstrerror(h_errno));
			continue;
		}
		printf("official host name: %s\n", hptr->h_name);

		for (pptr = hptr->h_aliases; *pptr != NULL; pptr++)
			printf("	alias: %s\n", *pptr);

		switch (hptr->h_addrtype) {
		case AF_INET:
			pr_ipv4(hptr->h_addr_list);
			break;

		default:
			err_ret("unknown address type");
			break;
		}
	}
	exit(0);
}

/*
 * Print the array of IPv4 addresses that is returned.
 * Also call gethostbyaddr_r() for each IP address and print the name.
 */

/* begin pr_ipv4 */
void
pr_ipv4(char **listptr)
{
	struct in_addr	inaddr;
	struct hostent	*hptr, hent;
	char			buf[8192];
	int				h_errno;

	for ( ; *listptr != NULL; listptr++) {
		inaddr = *((struct in_addr *) (*listptr));
		printf("	IPv4 address: %s", Inet_ntoa(inaddr));

		if ( (hptr = gethostbyaddr_r((char *) &inaddr, sizeof(struct in_addr),
									 AF_INET, &hent,
									 buf, sizeof(buf), &h_errno)) == NULL)
			printf("    (gethostbyaddr failed: %s)\n", hstrerror(h_errno));
		else if (hptr->h_name != NULL)
			printf("    name = %s\n", hptr->h_name);
		else
			printf("    (no hostname returned by gethostbyaddr)\n");
	}
}
/* end pr_ipv4 */
