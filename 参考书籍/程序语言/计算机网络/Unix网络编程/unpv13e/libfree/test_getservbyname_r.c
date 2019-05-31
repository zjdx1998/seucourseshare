#include	<netdb.h>

main()
{
	char		buf[8192];
	struct servent	sent, *sptr;

	sptr = getservbyname_r("tftp", "tcp", &sent, buf, sizeof(buf));
	printf("TCP, sptr = %p\n", sptr);

	sptr = getservbyname_r("tftp", "udp", &sent, buf, sizeof(buf));
	printf("UDP, sptr = %p\n", sptr);

	sptr = getservbyname_r("tftp", "tcp", &sent, buf, sizeof(buf));
	printf("TCP, sptr = %p\n", sptr);

	sptr = getservbyname_r("tftp", "udp", &sent, buf, sizeof(buf));
	printf("UDP, sptr = %p\n", sptr);

	exit(0);
}
