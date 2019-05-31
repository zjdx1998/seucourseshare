/*
 * If your system does not provide snprintf(), this program will compile
 * but will have an undefined error from the linker.
 *
 * If you are using the lib/snprintf.c function that is supplied for
 * systems without snprintf(), you should get an error output from our
 * library function.
 *
 * If your system provides the function, and it works, there should be
 * no output.
 */

#include	"unp.h"

int
main(int argc, char **argv)
{
	int		n;
	char	buf[1024];

	n = snprintf(buf, 4, "%d", 9999);
	if (n > 3)
		printf("error: snprintf overflowed buffer, n = %d\n", n);
	exit(0);
}
