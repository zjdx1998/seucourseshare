#include	"unp.h"
#include	<stropts.h>

int
main(int argc, char *argv[])
{
	int					fd, i, nmods;
	struct str_list		list;

	if (argc != 2)
		err_quit("usage: a.out { tcp | udp }");

	fd = Socket(AF_INET, (strcmp(argv[1], "tcp") == 0)
									? SOCK_STREAM : SOCK_DGRAM, 0);
	if (isastream(fd) == 0)
		err_quit("%s is not a stream", argv[1]);

	list.sl_nmods = nmods = Ioctl(fd, I_LIST, (void *) 0);
	printf("%d modules\n", nmods);
	list.sl_modlist = Calloc(nmods, sizeof(struct str_mlist));

	Ioctl(fd, I_LIST, &list);

	for (i = 1; i <= nmods; i++)
		printf("  %s: %s\n", (i == nmods) ? "driver" : "module",
								list.sl_modlist++);
	exit(0);
}
