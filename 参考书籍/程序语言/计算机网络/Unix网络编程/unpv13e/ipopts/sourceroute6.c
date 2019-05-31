#include	"unp.h"

void
inet6_srcrt_print(void *ptr)
{
	int			i, segments;
	char			str[INET6_ADDRSTRLEN];

	segments = Inet6_rth_segments(ptr);
	printf("received source route: ");
	for (i = 0; i < segments; i++)
		printf("%s ", Inet_ntop(AF_INET6, Inet6_rth_getaddr(ptr, i),
								str, sizeof(str)));
	printf("\n");
}
