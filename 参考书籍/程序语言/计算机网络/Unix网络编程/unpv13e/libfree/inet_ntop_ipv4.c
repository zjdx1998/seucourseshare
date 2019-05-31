#include	<sys/types.h>
#include	<sys/socket.h>
#include	<errno.h>
#include	<stdio.h>

#ifndef	INET_ADDRSTRLEN
#define	INET_ADDRSTRLEN		16
#endif

/* include inet_ntop */
const char *
inet_ntop(int family, const void *addrptr, char *strptr, size_t len)
{
	const u_char *p = (const u_char *) addrptr;

	if (family == AF_INET) {
		char	temp[INET_ADDRSTRLEN];

		snprintf(temp, sizeof(temp), "%d.%d.%d.%d",
				 p[0], p[1], p[2], p[3]);
		if (strlen(temp) >= len) {
			errno = ENOSPC;
			return (NULL);
		}
		strcpy(strptr, temp);
		return (strptr);
	}
	errno = EAFNOSUPPORT;
	return (NULL);
}
/* end inet_ntop */
