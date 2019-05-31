#include	"unp.h"

/*
 * This is a placeholder if the system does not provide this RFC 2133
 * function.  If routing sockets with sysctl() are provided, then the
 * if_XXX() functions in the libroute/ directory will replace these.
 */

unsigned int
if_nametoindex(const char *name)
{
	return(0);
}

unsigned int
If_nametoindex(const char *name)
{
	int		index;

	if ( (index = if_nametoindex(name)) == 0)
		err_quit("if_nametoindex error for %s", name);
	return(index);
}
