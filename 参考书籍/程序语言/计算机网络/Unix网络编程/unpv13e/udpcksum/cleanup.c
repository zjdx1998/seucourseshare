#include	"udpcksum.h"

/* include cleanup */
void
cleanup(int signo)
{
	struct pcap_stat	stat;

	putc('\n', stdout);

	if (verbose) {
		if (pcap_stats(pd, &stat) < 0)
			err_quit("pcap_stats: %s\n", pcap_geterr(pd));
		printf("%d packets received by filter\n", stat.ps_recv);
		printf("%d packets dropped by kernel\n", stat.ps_drop);
	}

	exit(0);
}
/* end cleanup */
