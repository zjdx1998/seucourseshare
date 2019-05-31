#include	"unp.h"
#include	<sys/param.h>
#include	<sys/ucred.h>

ssize_t	recv_cred(int, void *, size_t, struct fcred *);

main()
{
	int				fd[2], on, n;
	char			buf[100];
	struct fcred	cred;

	if (socketpair(AF_LOCAL, SOCK_STREAM, 0, fd) < 0)
		err_sys("socketpair error");

		/* must set the socket option on the *receiving* socket */
	on = 1;
	Setsockopt(fd[1], 0, LOCAL_CREDS, &on, sizeof(on));

	Write(fd[0], "hello, world\n", 13);

	if ( (n = recv_cred(fd[1], buf, sizeof(buf), &cred)) < 0)
		err_sys("recv_cred error");
	else if (n == 0)
		err_quit("recv_cred, unexpected EOF");

	buf[n] = 0;			/* null terminate */
	printf("data: %s", buf);

	if (cred.fc_ngroups == 0)
		printf("(no credentials returned)\n");
	else {
		printf("real user ID = %d\n", cred.fc_ruid);
		printf("real group ID = %d\n", cred.fc_rgid);
		printf("login name = %-*s\n", MAXLOGNAME, cred.fc_login);
		printf("effective user ID = %d\n", cred.fc_uid);
		printf("effective group ID = %d\n", cred.fc_gid);
		printf("%d supplementary groups:", cred.fc_ngroups - 1);
		for (n = 1; n < cred.fc_ngroups; n++)	/* [0] is the egid */
			printf(" %d", cred.fc_groups[n]);
		printf("\n");
	}

	exit(0);
}

#define	CONTROL_LEN	(sizeof(struct cmsghdr) + sizeof(struct fcred))

ssize_t
recv_cred(int fd, void *ptr, size_t nbytes, struct fcred *fcredptr)
{
	struct msghdr	msg;
	struct iovec	iov[1];
	char			control[CONTROL_LEN + 20];
	int				n;

	msg.msg_name = NULL;
	msg.msg_namelen = 0;
	iov[0].iov_base = ptr;
	iov[0].iov_len = nbytes;
	msg.msg_iov = iov;
	msg.msg_iovlen = 1;
	msg.msg_control = control;
	msg.msg_controllen = sizeof(control);
	msg.msg_flags = 0;

	if ( (n = recvmsg(fd, &msg, 0)) < 0)
		return(n);

	fcredptr->fc_ngroups = 0;	/* indicates no credentials returned */
	if (fcredptr && msg.msg_controllen > 0) {
		struct cmsghdr	*cmptr = (struct cmsghdr *) control;

		if (cmptr->cmsg_len != sizeof(struct cmsghdr) + sizeof(struct fcred))
			err_quit("control length = %d", cmptr->cmsg_len);
		if (cmptr->cmsg_level != SOL_SOCKET)
			err_quit("control level != SOL_SOCKET");
		if (cmptr->cmsg_type != SCM_CREDS)
			err_quit("control type != SCM_CREDS");
		memcpy(fcredptr, CMSG_DATA(cmptr), sizeof(struct fcred));
	}

	return(n);
}
