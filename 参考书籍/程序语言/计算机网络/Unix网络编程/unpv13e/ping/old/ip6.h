struct ip6hdr {
    union {
	struct ip6hdrctl {
	    u_int32_t ctl6_flow;	/* 24 bits of flow-ID */
	    u_int16_t ctl6_plen;        /* payload length */
	    u_int8_t  ctl6_nxt;         /* next header */
	    u_int8_t  ctl6_hlim;        /* hop limit */
	} un_ctl6;
	u_int8_t un_vfc;		/* 4 bits version, 4 bits reserved */
    } ip6_ctlun;
    struct in6_addr ip6_src;		/* source address */
    struct in6_addr ip6_dst;		/* destination address */
};

#define ip6_vfc		ip6_ctlun.un_vfc
#define ip6_flow	ip6_ctlun.un_ctl6.ctl6_flow
#define	ip6_plen	ip6_ctlun.un_ctl6.ctl6_plen
#define	ip6_nxt		ip6_ctlun.un_ctl6.ctl6_nxt
#define	ip6_hlim	ip6_ctlun.un_ctl6.ctl6_hlim
#define	ip6_hops	ip6_ctlun.un_ctl6.ctl6_hlim
