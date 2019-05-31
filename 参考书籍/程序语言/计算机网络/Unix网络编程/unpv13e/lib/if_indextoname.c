#include	"unp.h"

/*
 * This is a placeholder if the system does not provide this RFC 2133
 * function.  If routing sockets with sysctl() are provided, then the
 * if_XXX() functions in the libroute/ directory will replace these.
 */

char *
if_indextoname(unsigned int index, char *name)
{
	return(NULL);
}

char *
If_indextoname(unsigned int index, char *name)
{
	char	*ptr;

	if ( (ptr = if_indextoname(index, name)) == NULL)
		err_quit("if_indextoname error for %d", index);
	return(ptr);
}
