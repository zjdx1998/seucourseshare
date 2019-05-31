#include	"unp.h"

int
main(int argc, char **argv)
{
	int		count = 0;
	ssize_t	n;
	char	recvline[MAXLINE];

	while ( ( n = readline(STDIN_FILENO, recvline, MAXLINE)) > 0)
		count++;
	printf("%d lines\n", count);
}
