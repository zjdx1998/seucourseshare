#include	"unpifi.h"

int
main(int argc, char **argv)
{
	int					n;
	char				ifname[IFNAMSIZ];
	struct if_nameindex	*ifptr, *save;

	if (argc != 1)
		err_quit("usage: prifnameindex");

		/* print all the interface names and indexes */
	for (save = ifptr = If_nameindex(); ifptr->if_index > 0; ifptr++) {
		printf("name = %s, index = %d\n", ifptr->if_name, ifptr->if_index);;

		if ( (n = If_nametoindex(ifptr->if_name)) != ifptr->if_index)
			err_quit("if_nametoindex returned %d, expected %d, for %s",
					 n, ifptr->if_index, ifptr->if_name);

		If_indextoname(ifptr->if_index, ifname);
		if (strcmp(ifname, ifptr->if_name) != 0)
			err_quit("if_indextoname returned %s, expected %s, for %d",
					 ifname, ifptr->if_name, ifptr->if_index);
	}

	n = if_nametoindex("fkjhkjhgjhgjhgdjhguyetiuyiuyhkjhkjdh");
	if (n != 0)
		err_quit("if_nametoindex returned %d for fkjh...", n);
	n = if_nametoindex("");
	if (n != 0)
		err_quit("if_nametoindex returned %d for (null)", n);

	if (if_indextoname(0, ifname) != NULL)
		err_quit("if_indextoname error for 0");
	if (if_indextoname(888888, ifname) != NULL)
		err_quit("if_indextoname error for 888888");

	if_freenameindex(save);
	exit(0);
}
