#include	"unp.h"
#include	<sys/utsname.h>

#define	SENDRATE	5		/* send one datagram every five seconds */

void
send_all(int sendfd, SA *sadest, socklen_t salen)
{
	char		line[MAXLINE];		/* hostname and process ID */
	struct utsname	myname;

	if (uname(&myname) < 0)
		err_sys("uname error");;
	snprintf(line, sizeof(line), "%s, %d\n", myname.nodename, getpid());

	for ( ; ; ) {
		Sendto(sendfd, line, strlen(line), 0, sadest, salen);

		sleep(SENDRATE);
	}
}
