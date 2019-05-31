/* include my_addrs */
#include	"unp.h"
#include	<sys/utsname.h>

char **
my_addrs(int *addrtype)
{
	struct hostent	*hptr;
	struct utsname	myname;

	if (uname(&myname) < 0)
		return(NULL);

	if ( (hptr = gethostbyname(myname.nodename)) == NULL)
		return(NULL);

	*addrtype = hptr->h_addrtype;
	return(hptr->h_addr_list);
}
/* end my_addrs */

char **
My_addrs(int *pfamily)
{
	char	**pptr;

	if ( (pptr = my_addrs(pfamily)) == NULL)
		err_sys("my_addrs error");
	return(pptr);
}
