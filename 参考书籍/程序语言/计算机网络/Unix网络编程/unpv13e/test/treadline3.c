#include	"unp.h"
#include	"readline.h"

int
main(int argc, char **argv)
{
	int		count = 0;
	ssize_t	n;
	char	recvline[MAXLINE];
	Rline	rline;

	readline_rinit(STDIN_FILENO, recvline, MAXLINE, &rline);
	while ( (n = readline_r(&rline)) > 0)
		count++;
	printf("%d lines\n", count);
}
