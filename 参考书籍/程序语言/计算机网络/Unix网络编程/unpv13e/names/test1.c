#include	"unp.h"

void	pr_ipv4(char **);

int
main(int argc, char **argv)
{
	char			*ptr, **pptr, **listptr, buf[INET6_ADDRSTRLEN];
	char			*list[100];
	int				i, addrtype, addrlen;
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
		addrtype = hptr->h_addrtype;
		addrlen = hptr->h_length;

			/* copy array of pointers, so we can call gethostbyaddr() */
		for (i = 0, listptr = hptr->h_addr_list; *listptr != NULL; listptr++) {
			list[i++] = *listptr;
		}
		list[i] = NULL;

		for (listptr = list; *listptr != NULL; listptr++) {
			printf("\taddress: %s\n",
				   Inet_ntop(addrtype, *listptr, buf, sizeof(buf)));

			if ( (hptr = gethostbyaddr(*listptr, addrlen, addrtype)) == NULL)
				printf("\t\t(gethostbyaddr failed)\n");
			else if (hptr->h_name != NULL)
				printf("\t\tname = %s\n", hptr->h_name);
			else
				printf("\t\t(no hostname returned by gethostbyaddr)\n");

			printf("\t\tofficial host name: %s\n", hptr->h_name);

			for (pptr = hptr->h_aliases; *pptr != NULL; pptr++)
				printf("\t\talias: %s\n", *pptr);
		}
	}
}
