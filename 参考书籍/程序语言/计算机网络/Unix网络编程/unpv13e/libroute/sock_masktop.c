#include	"unproute.h"

const char *
sock_masktop(SA *sa, socklen_t salen)
{
	static char		str[INET6_ADDRSTRLEN];
	unsigned char	*ptr = &sa->sa_data[2];

	if (sa->sa_len == 0)
		return("0.0.0.0");
	else if (sa->sa_len == 5)
		snprintf(str, sizeof(str), "%d.0.0.0", *ptr);
	else if (sa->sa_len == 6)
		snprintf(str, sizeof(str), "%d.%d.0.0", *ptr, *(ptr+1));
	else if (sa->sa_len == 7)
		snprintf(str, sizeof(str), "%d.%d.%d.0", *ptr, *(ptr+1), *(ptr+2));
	else if (sa->sa_len == 8)
		snprintf(str, sizeof(str), "%d.%d.%d.%d",
				 *ptr, *(ptr+1), *(ptr+2), *(ptr+3));
	else
		snprintf(str, sizeof(str), "(unknown mask, len = %d, family = %d)",
				 sa->sa_len, sa->sa_family);
	return(str);
}
