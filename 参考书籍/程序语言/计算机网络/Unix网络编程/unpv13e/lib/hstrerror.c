/*
 * Return a string containing some additional information after a
 * host name or address lookup error - gethostbyname() or gethostbyaddr().
 *
 * This is only compiled if the local host does not provide it--recent
 * versions of BIND supply this function.
 */

#include	"unp.h"

const char *
hstrerror(int err)
{
	if (err == 0)
		return("no error");

	if (err == HOST_NOT_FOUND)
		return("Unknown host");

	if (err == TRY_AGAIN)
		return("Hostname lookup failure");

	if (err == NO_RECOVERY)
		return("Unknown server error");

	if (err == NO_DATA)
        return("No address associated with name");

	return("unknown error");
}
