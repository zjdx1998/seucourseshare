#include	"unproute.h"
#include	<netinet/udp.h>
#include	<netinet/ip_var.h>
#include	<netinet/udp_var.h>		/* for UDPCTL_xxx constants */

int
main(int argc, char **argv)
{
	int		mib[4], val;
	size_t	len;

	mib[0] = CTL_NET;
	mib[1] = AF_INET;
	mib[2] = IPPROTO_UDP;
	mib[3] = UDPCTL_CHECKSUM;

	len = sizeof(val);
	Sysctl(mib, 4, &val, &len, NULL, 0);
	printf("udp checksum flag: %d\n", val);

	exit(0);
}
