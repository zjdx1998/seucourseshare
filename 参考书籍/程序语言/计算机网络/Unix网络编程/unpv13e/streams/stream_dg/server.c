#include <stdio.h>
#include <sys/types.h>
#include <sys/fcntl.h>
#include <sys/errno.h>

#include <sys/socket.h>
#include <netinet/in.h>

extern int errno;

#define SERV_TCP_PORT   6000

main()
{
	int fd;
	struct sockaddr_in	my_addr;
	void			echo_serv (int);

	bzero((char *) &my_addr, sizeof(my_addr));
	my_addr.sin_family      = AF_INET;
	my_addr.sin_addr.s_addr = htonl (INADDR_ANY);
	my_addr.sin_port        = htons(SERV_TCP_PORT);

	fd = net_open ("/dev/udp", O_RDWR);
	fd = net_bind (fd, &my_addr, sizeof (struct sockaddr_in));

	echo_serv (fd);
	exit (0);	/* not reached */
}

void
echo_serv (int fd)
{
	struct sockaddr_in	fm_addr;
	char			buf[512];
	int			n;

	while (1)
	{
		n = net_recv (fd, buf, sizeof (buf),
				&fm_addr, sizeof (struct sockaddr_in));

		net_send (fd, buf, n,
				&fm_addr, sizeof (struct sockaddr_in));
	}
}
