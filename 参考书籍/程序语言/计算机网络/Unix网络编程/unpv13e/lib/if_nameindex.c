#include	"unp.h"

/*
 * This is a placeholder if the system does not provide this RFC 2133
 * function.  If routing sockets with sysctl() are provided, then the
 * if_XXX() functions in the libroute/ directory will replace these.
 */

struct if_nameindex *
if_nameindex(void)
{
	return(NULL);
}

void
if_freenameindex(struct if_nameindex *ptr)
{
}

struct if_nameindex *
If_nameindex(void)
{
	struct if_nameindex	*ifptr;

	if ( (ifptr = if_nameindex()) == NULL)
		err_quit("if_nameindex error");
	return(ifptr);
}
