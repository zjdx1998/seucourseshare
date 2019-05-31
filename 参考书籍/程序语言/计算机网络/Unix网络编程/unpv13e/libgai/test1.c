#include	"unp.h"

/*
 * See that gethostbyname2() with an address string as the first argument
 * is broken in BIND-8.1-REL.
 * gethostbyname2("127.0.0.1", AF_INET6) -> OK!
 */

int
main(int argc, char **argv)
{
	struct hostent	*hptr;

	if (argc != 2)
		err_quit("usage: test2 <IPaddress>");

	printf("gethostbyname2(%s, AF_INET): ", argv[1]);
	hptr = gethostbyname2(argv[1], AF_INET);
	printf("%s\n", (hptr == NULL) ? "failed" : "OK");

#ifdef	IPv6
	printf("gethostbyname2(%s, AF_INET6): ", argv[1]);
	hptr = gethostbyname2(argv[1], AF_INET6);
	printf("%s\n", (hptr == NULL) ? "failed" : "OK");
#endif

	exit(0);
}
