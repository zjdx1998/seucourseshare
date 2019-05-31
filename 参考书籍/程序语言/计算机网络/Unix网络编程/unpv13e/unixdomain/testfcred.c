#include	"unp.h"
#include	<sys/param.h>
#include	<sys/ucred.h>

main()
{
	printf("sizeof(struct fcred) = %d\n", sizeof(struct fcred));
	printf("sizeof(struct cmsghdr) = %d\n", sizeof(struct cmsghdr));
	exit(0);
}
