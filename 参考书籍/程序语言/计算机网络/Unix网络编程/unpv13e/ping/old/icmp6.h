struct icmp6hdr {
    u_int8_t	icmp6_type;		/* Type field */
    u_int8_t	icmp6_code;		/* Code field */
    u_int16_t	icmp6_cksum;		/* Checksum field */
    union {
	u_int32_t	un_data32[1];	/* Type-specific field */
	u_int16_t	un_data16[2];	/* Type-specific field */
	u_int8_t	un_data8[4];	/* Type-specific field */
    } icmp6_dataun;
};

#define	icmp6_data32	icmp6_dataun.un_data32
#define	icmp6_data16	icmp6_dataun.un_data16
#define	icmp6_data8	icmp6_dataun.un_data8
#define	icmp6_pptr	icmp6_data32[0]		/* PARAMPROB */
#define	icmp6_mtu	icmp6_data32[0]		/* PKT_TOOBIG */
#define	icmp6_id	icmp6_data16[0]		/* ECHO */
#define	icmp6_seq	icmp6_data16[1]		/* ECHO */
#define	icmp6_maxdelay	icmp6_data16[0]		/* MGM_xxx */

#define	ICMPV6_DEST_UNREACH	1
#define	ICMPV6_PKT_TOOBIG	2
#define	ICMPV6_TIME_EXCEED	3
#define	ICMPV6_PARAMPROB	4

#define	ICMPV6_INFOMSG_MASK	0x80	/* all informational messages */

#define	ICMPV6_ECHORQST		128
#define	ICMPV6_ECHORPLY		129
#define	ICMPV6_MGM_QUERY	130
#define	ICMPV6_MGM_REPORT	131
#define	ICMPV6_MGM_REDUCTION	132

#define	ICMPV6_DEST_UNREACH_NOROUTE	0 /* no route to destination */
#define	ICMPV6_DEST_UNREACH_ADMIN 	1 /* communication with destination */
					  /*  administratively prohibited */
#define	ICMPV6_DEST_UNREACH_NOTNEIGHBOR	2 /* not a neighbor */
#define	ICMPV6_DEST_UNREACH_ADDR 	3 /* address unreachable */
#define	ICMPV6_DEST_UNREACH_NOPORT	4 /* bad port */

#define	ICMPV6_TIME_EXCEED_HOPS		0 /* Hop Limit == 0 in transit */
#define	ICMPV6_TIME_EXCEED_REASSEMBLY	1 /* Reassembly time out */

#define	ICMPV6_PARAMPROB_HDR		0 /* erroneous header field */
#define	ICMPV6_PARAMPROB_NXT_HDR	1 /* unrecognized Next Header */
#define	ICMPV6_PARAMPROB_OPTS		2 /* unrecognized IPv6 option */
