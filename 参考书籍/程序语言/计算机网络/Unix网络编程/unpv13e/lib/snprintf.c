/*
 * Throughout the book I use snprintf() because it's safer than sprintf().
 * But as of the time of this writing, not all systems provide this
 * function.  The function below should only be built on those systems
 * that do not provide a real snprintf().
 * The function below just acts like sprintf(); it is not safe, but it
 * tries to detect overflow.
 */

#include	"unp.h"

#include	<stdarg.h>		/* ANSI C header file */

int
snprintf(char *buf, size_t size, const char *fmt, ...)
{
	int			n;
	va_list		ap;

	va_start(ap, fmt);
	vsprintf(buf, fmt, ap);	/* Sigh, some vsprintf's return ptr, not length */
	n = strlen(buf);
	va_end(ap);
	if (n >= size)
		err_quit("snprintf: '%s' overflowed array", fmt);
	return(n);
}
