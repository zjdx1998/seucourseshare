#include	"unp.h"

int
sockatmark(int fd)
{
	int		flag;

	if (ioctl(fd, SIOCATMARK, &flag) < 0)
		return(-1);
	return(flag != 0);
}
