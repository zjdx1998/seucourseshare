#include	"../lib/unp.h"

#ifndef	AF_INET6
#define	AF_INET6	AF_MAX+1	/* just to let this compile */
#endif

int		inet_pton(int, const char *, void *);

int
main(int argc, char **argv)
{
	int		i;
	char	buff[100];

	/*
	 * Make certain that we can test the difference between 0.0.0.0
	 * being acceptable for AF_INET (but not acceptable for AF_INET6)
	 * and 0::0 being OK for AF_INET6 (but not OK for AF_INET).
	 * This way a server can be coded as protocol independent (IPv4 or
	 * IPv6) but let the user specify the local IP address as either
	 * 0.0.0.0 or 0::0 as an indirect way of telling the server when
	 * it starts, which protocol to use (but still allowing the server
	 * to bind the wildcard address).
	 */

	if ( (i = inet_pton(AF_INET, "0.0.0.0", buff)) != 1)	/* should be OK */
		printf("AF_INET, 0.0.0.0 returned: %d\n", i);
	if ( (i = inet_pton(AF_INET6, "0.0.0.0", buff)) != 0)
		printf("AF_INET6, 0.0.0.0 returned: %d\n", i);

	if ( (i = inet_pton(AF_INET6, "0::0", buff)) != 1)		/* should be OK */
		printf("AF_INET6, 0::0 returned: %d\n", i);
	if ( (i = inet_pton(AF_INET, "0::0", buff)) != 0)
		printf("AF_INET, 0::0 returned: %d\n", i);

	printf("inet_pton(AF_INET6, \"1.2.3.4\", buff) returns %d\n",
		    inet_pton(AF_INET6,  "1.2.3.4", buff));
	printf("inet_pton(AF_INET6, \"::1.2.3.4\", buff) returns %d\n",
		    inet_pton(AF_INET6,  "::1.2.3.4", buff));

	exit(0);
}
