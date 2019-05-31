#include	"unpxti.h"

int
main(int argc, char **argv)
{
	int				fd;
	struct t_call	*tcall;

	fd = T_open(XTI_TCP, O_RDWR, NULL);

	tcall = T_alloc(fd, T_CALL, T_ALL);
	printf("first t_alloc OK\n");

	tcall = T_alloc(fd, T_CALL, T_ADDR | T_OPT | T_UDATA);
	printf("second t_alloc OK\n");

	exit(0);
}
