#include "unp.h"
#include <net/pfkeyv2.h>

/* include sadb_dump */
void
sadb_dump(int type)
{
	int s;
	char buf[4096];
	struct sadb_msg msg;
	int goteof;

	s = Socket(PF_KEY, SOCK_RAW, PF_KEY_V2);

	/* Build and write SADB_DUMP request */
	bzero(&msg, sizeof(msg));
	msg.sadb_msg_version = PF_KEY_V2;
	msg.sadb_msg_type = SADB_DUMP;
	msg.sadb_msg_satype = type;
	msg.sadb_msg_len = sizeof(msg) / 8;
	msg.sadb_msg_pid = getpid();
	printf("Sending dump message:\n");
	print_sadb_msg(&msg, sizeof(msg));
	Write(s, &msg, sizeof(msg));

	printf("\nMessages returned:\n");
	/* Read and print SADB_DUMP replies until done */
	goteof = 0;
	while (goteof == 0) {
		int msglen;
		struct sadb_msg *msgp;

		msglen = Read(s, &buf, sizeof(buf));
		msgp = (struct sadb_msg *)&buf;
		print_sadb_msg(msgp, msglen);
		if (msgp->sadb_msg_seq == 0)
			goteof = 1;
	}
	close(s);
}

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

	sadb_dump(satype);
}
/* end sadb_dump */
