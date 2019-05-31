#include "unp.h"
#include <net/pfkeyv2.h>

/* include sadb_register */
void
sadb_register(int type)
{
	int s;
	char buf[4096];	/* XXX */
	struct sadb_msg msg;
	int goteof;
	int mypid;

	s = Socket(PF_KEY, SOCK_RAW, PF_KEY_V2);

	mypid = getpid();

	/* Build and write SADB_REGISTER request */
	bzero(&msg, sizeof(msg));
	msg.sadb_msg_version = PF_KEY_V2;
	msg.sadb_msg_type = SADB_REGISTER;
	msg.sadb_msg_satype = type;
	msg.sadb_msg_len = sizeof(msg) / 8;
	msg.sadb_msg_pid = mypid;
	printf("Sending register message:\n");
	print_sadb_msg(&msg, sizeof(msg));
	Write(s, &msg, sizeof(msg));

	printf("\nReply returned:\n");
	/* Read and print SADB_REGISTER reply, discarding any others */
	for (;;) {
		int msglen;
		struct sadb_msg *msgp;

		msglen = Read(s, &buf, sizeof(buf));
		msgp = (struct sadb_msg *)&buf;
		if (msgp->sadb_msg_pid == mypid &&
			msgp->sadb_msg_type == SADB_REGISTER) {
			print_sadb_msg(msgp, msglen);
			break;
		}
	}
	close(s);
}
/* end sadb_register */

int
main(int argc, char **argv)
{
	int satype = SADB_SATYPE_UNSPEC;
	int c;

	opterr = 0;		/* don't want getopt() writing to stderr */
	while ( (c = getopt(argc, argv, "t:")) != -1) {
		switch (c) {
		case 't':
			if ((satype = getsatypebyname(optarg)) == -1)
				err_quit("invalid -t option %s", optarg);
			break;

		default:
			err_quit("unrecognized option: %c", c);
		}
	}

	if (satype == SADB_SATYPE_UNSPEC) {
		err_quit("must specify SA type");
	}

	sadb_register(satype);
}
