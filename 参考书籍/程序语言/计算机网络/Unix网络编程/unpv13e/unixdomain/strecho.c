#include	"unp.h"

ssize_t	read_cred(int, void *, size_t, struct cmsgcred *);

void
str_echo(int sockfd)
{
	ssize_t			n;
	int			i;
	char			buf[MAXLINE];
	struct cmsgcred	cred;

again:
	while ( (n = read_cred(sockfd, buf, MAXLINE, &cred)) > 0) {
		if (cred.cmcred_ngroups == 0) {
			printf("(no credentials returned)\n");
		} else {
			printf("PID of sender = %d\n", cred.cmcred_pid);
			printf("real user ID = %d\n", cred.cmcred_uid);
			printf("real group ID = %d\n", cred.cmcred_gid);
			printf("effective user ID = %d\n", cred.cmcred_euid);
			printf("%d groups:", cred.cmcred_ngroups - 1);
			for (i = 1; i < cred.cmcred_ngroups; i++)
				printf(" %d", cred.cmcred_groups[i]);
			printf("\n");
		}
		Writen(sockfd, buf, n);
	}

	if (n < 0 && errno == EINTR)
		goto again;
	else if (n < 0)
		err_sys("str_echo: read error");
}
