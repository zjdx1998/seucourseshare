#include	"udpcksum.h"
#include	<libnet.h>

/*
 * Build a DNS A query for "a.root-servers.net" and write it to
 * the raw socket.
 */

/* include open_output_libnet */
static libnet_t *l;		/* libnet descriptor */

void
open_output(void)
{
	char errbuf[LIBNET_ERRBUF_SIZE];

	/* Initialize libnet with an IPv4 raw socket */
	l = libnet_init(LIBNET_RAW4, NULL, errbuf);
	if (l == NULL) {
		err_quit("Can't initialize libnet: %s", errbuf);
	}
}
/* end open_output_libnet */

/* include send_dns_query_libnet */
void
send_dns_query(void)
{
	char				 qbuf[24], *ptr;
	u_int16_t			 one;
	int					 packet_size = LIBNET_UDP_H + LIBNET_DNSV4_H + 24;
	static libnet_ptag_t ip_tag, udp_tag, dns_tag;

	/* build query portion of DNS packet */
	ptr = qbuf;
	memcpy(ptr, "\001a\014root-servers\003net\000", 20);
	ptr += 20;
	one = htons(1);
	memcpy(ptr, &one, 2);				/* query type = A */
	ptr += 2;
	memcpy(ptr, &one, 2);				/* query class = 1 (IP addr) */

	/* build DNS packet */
	dns_tag = libnet_build_dnsv4(
			1234 /* identification */,
			0x0100 /* flags: recursion desired */,
			1 /* # questions */, 	0 /* # answer RRs */,
			0 /* # authority RRs */, 0 /* # additional RRs */,
			qbuf /* query */, 24 /* length of query */, l, dns_tag);
	/* build UDP header */
	udp_tag = libnet_build_udp(
			((struct sockaddr_in *) local)->sin_port /* source port */,
			((struct sockaddr_in *) dest)->sin_port /* dest port */,
			packet_size /* length */, 0 /* checksum */,
			NULL /* payload */, 0 /* payload length */, l, udp_tag);
	/* Since we specified the checksum as 0, libnet will automatically */
	/* calculate the UDP checksum.  Turn it off if the user doesn't want it. */
	if (zerosum)
		if (libnet_toggle_checksum(l, udp_tag, LIBNET_OFF) < 0)
			err_quit("turning off checksums: %s\n", libnet_geterror(l));
	/* build IP header */
/* *INDENT-OFF* */
	ip_tag = libnet_build_ipv4(packet_size + LIBNET_IPV4_H /* len */,
			0 /* tos */, 0 /* IP ID */, 0 /* fragment */,
			TTL_OUT /* ttl */, IPPROTO_UDP /* protocol */,
			0 /* checksum */,
			((struct sockaddr_in *) local)->sin_addr.s_addr /* source */,
			((struct sockaddr_in *) dest)->sin_addr.s_addr /* dest */,
			NULL /* payload */, 0 /* payload length */, l, ip_tag);
/* *INDENT-ON* */

	if (libnet_write(l) < 0) {
		err_quit("libnet_write: %s\n", libnet_geterror(l));
	}
	if (verbose)
		printf("sent: %d bytes of data\n", packet_size);
}
/* end send_dns_query_libnet */
