#include	"unp.h"
#include	<net/if.h>

int
main(int argc, char **argv)
{
	int		i, sockfd, numif;
	char	buf[1024];

	sockfd = Socket(AF_INET, SOCK_DGRAM, 0);

	numif = 999;
	Ioctl(sockfd, SIOCGIFNUM, &numif);
	printf("numif = %d\n", numif);

	i = ioctl(sockfd, SIOCGHIWAT, &buf);
	printf("i = %d, errno = %d\n", i, errno);
	exit(0);
}
