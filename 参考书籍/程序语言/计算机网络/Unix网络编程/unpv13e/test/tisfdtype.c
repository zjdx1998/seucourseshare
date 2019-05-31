#include	"unp.h"

main()
{
	int	tcpsock, udpsock;

	printf("stdin: %d\n", Isfdtype(STDIN_FILENO, S_IFSOCK));
	printf("stdout: %d\n", Isfdtype(STDOUT_FILENO, S_IFSOCK));
	printf("stderr: %d\n", Isfdtype(STDERR_FILENO, S_IFSOCK));

	tcpsock = Socket(AF_INET, SOCK_STREAM, 0);
	printf("TCP socket: %d\n", Isfdtype(tcpsock, S_IFSOCK));

	udpsock = Socket(AF_INET, SOCK_DGRAM, 0);
	printf("UDP socket: %d\n", Isfdtype(udpsock, S_IFSOCK));

	exit(0);
}
