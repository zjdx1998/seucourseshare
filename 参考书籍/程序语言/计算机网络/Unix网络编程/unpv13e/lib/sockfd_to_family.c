/* include sockfd_to_family */
#include	"unp.h"

int
sockfd_to_family(int sockfd)
{
	struct sockaddr_storage ss;
	socklen_t	len;

	len = sizeof(ss);
	if (getsockname(sockfd, (SA *) &ss, &len) < 0)
		return(-1);
	return(ss.ss_family);
}
/* end sockfd_to_family */

int
Sockfd_to_family(int sockfd)
{
	int		rc;

	if ( (rc = sockfd_to_family(sockfd)) < 0)
		err_sys("sockfd_to_family error");

	return(rc);
}
