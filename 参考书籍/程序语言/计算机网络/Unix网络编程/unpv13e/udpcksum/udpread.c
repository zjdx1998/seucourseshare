#include	"udpcksum.h"

struct udpiphdr	*udp_check(char *, int);

/*
 * Read from the network until a UDP datagram is read that matches
 * the arguments.
 */

/* include udp_read */
struct udpiphdr *
udp_read(void)
{
	int					len;
	char				*ptr;
	struct ether_header	*eptr;

	for ( ; ; ) {
		ptr = next_pcap(&len);

		switch (datalink) {
		case DLT_NULL:	/* loopback header = 4 bytes */
			return(udp_check(ptr+4, len-4));

		case DLT_EN10MB:
			eptr = (struct ether_header *) ptr;
			if (ntohs(eptr->ether_type) != ETHERTYPE_IP)
				err_quit("Ethernet type %x not IP", ntohs(eptr->ether_type));
			return(udp_check(ptr+14, len-14));

		case DLT_SLIP:	/* SLIP header = 24 bytes */
			return(udp_check(ptr+24, len-24));

		case DLT_PPP:	/* PPP header = 24 bytes */
			return(udp_check(ptr+24, len-24));

		default:
			err_quit("unsupported datalink (%d)", datalink);
		}
	}
}
/* end udp_read */

/*
 * Check the received packet.
 * If UDP and OK, return pointer to packet.
 * If ICMP error, return NULL.
 * We assume the filter picks out desired UDP datagrams.
 */

/* include udp_check */
struct udpiphdr *
udp_check(char *ptr, int len)
{
	int					hlen;
	struct ip			*ip;
	struct udpiphdr		*ui;
/* *INDENT-OFF* */

	if (len < sizeof(struct ip) + sizeof(struct udphdr))
		err_quit("len = %d", len);
/* *INDENT-ON* */

		/* 4minimal verification of IP header */
	ip = (struct ip *) ptr;
	if (ip->ip_v != IPVERSION)
		err_quit("ip_v = %d", ip->ip_v);
	hlen = ip->ip_hl << 2;
/* *INDENT-OFF* */
	if (hlen < sizeof(struct ip))
		err_quit("ip_hl = %d", ip->ip_hl);
	if (len < hlen + sizeof(struct udphdr))
		err_quit("len = %d, hlen = %d", len, hlen);
/* *INDENT-ON* */

	if ( (ip->ip_sum = in_cksum((uint16_t *) ip, hlen)) != 0)
		err_quit("ip checksum error");

	if (ip->ip_p == IPPROTO_UDP) {
		ui = (struct udpiphdr *) ip;
		return(ui);
	} else
		err_quit("not a UDP packet");
}
/* end udp_check */
