#include "unp.h"
#include <net/pfkeyv2.h>

int
salen(struct sockaddr *sa)
{
#ifdef HAVE_SOCKADDR_SA_LEN
	return sa->sa_len;
#else
	switch (sa->sa_family) {
	case AF_INET:
		return sizeof(struct sockaddr_in);
#ifdef IPV6
	case AF_INET6:
		return sizeof(struct sockaddr_in6);
#endif
	default:
		return 0;	/* XXX */
	}
#endif
}

int
prefix_all(struct sockaddr *sa)
{
	switch (sa->sa_family) {
	case AF_INET:
		return 32;
#ifdef IPV6
	case AF_INET6:
		return 128;
#endif
	default:
		return 0;	/* XXX */
	}
}

/* include sadb_add */
void
sadb_add(struct sockaddr *src, struct sockaddr *dst, int type, int alg,
		int spi, int keybits, unsigned char *keydata)
{
	int s;
	char buf[4096], *p;	/* XXX */
	struct sadb_msg *msg;
	struct sadb_sa *saext;
	struct sadb_address *addrext;
	struct sadb_key *keyext;
	int len;
	int mypid;

	s = Socket(PF_KEY, SOCK_RAW, PF_KEY_V2);

	mypid = getpid();

	/* Build and write SADB_ADD request */
	bzero(&buf, sizeof(buf));
	p = buf;
	msg = (struct sadb_msg *)p;
	msg->sadb_msg_version = PF_KEY_V2;
	msg->sadb_msg_type = SADB_ADD;
	msg->sadb_msg_satype = type;
	msg->sadb_msg_pid = getpid();
	len = sizeof(*msg);
	p += sizeof(*msg);
	 
	saext = (struct sadb_sa *)p;
	saext->sadb_sa_len = sizeof(*saext) / 8;
	saext->sadb_sa_exttype = SADB_EXT_SA;
	saext->sadb_sa_spi = htonl(spi);
	saext->sadb_sa_replay = 0;	/* no replay protection with static keys */
	saext->sadb_sa_state = SADB_SASTATE_MATURE;
	saext->sadb_sa_auth = alg;
	saext->sadb_sa_encrypt = SADB_EALG_NONE;
	saext->sadb_sa_flags = 0;
	len += saext->sadb_sa_len * 8;
	p += saext->sadb_sa_len * 8;

	addrext = (struct sadb_address *)p;
	addrext->sadb_address_len = (sizeof(*addrext) + salen(src) + 7) / 8;
	addrext->sadb_address_exttype = SADB_EXT_ADDRESS_SRC;
	addrext->sadb_address_proto = 0;	/* any protocol */
	addrext->sadb_address_prefixlen = prefix_all(src);
	addrext->sadb_address_reserved = 0;
	memcpy(addrext + 1, src, salen(src));
	len += addrext->sadb_address_len * 8;
	p += addrext->sadb_address_len * 8;
	 
	addrext = (struct sadb_address *)p;
	addrext->sadb_address_len = (sizeof(*addrext) + salen(dst) + 7) / 8;
	addrext->sadb_address_exttype = SADB_EXT_ADDRESS_DST;
	addrext->sadb_address_proto = 0;	/* any protocol */
	addrext->sadb_address_prefixlen = prefix_all(dst);
	addrext->sadb_address_reserved = 0;
	memcpy(addrext + 1, dst, salen(dst));
	len += addrext->sadb_address_len * 8;
	p += addrext->sadb_address_len * 8;

	keyext = (struct sadb_key *)p;
	/* "+7" handles alignment requirements */
	keyext->sadb_key_len = (sizeof(*keyext) + (keybits / 8) + 7) / 8;
	keyext->sadb_key_exttype = SADB_EXT_KEY_AUTH;
	keyext->sadb_key_bits = keybits;
	keyext->sadb_key_reserved = 0;
	memcpy(keyext + 1, keydata, keybits / 8);
	len += keyext->sadb_key_len * 8;
	p += keyext->sadb_key_len * 8;
	 
	msg->sadb_msg_len = len / 8;
	printf("Sending add message:\n");
	print_sadb_msg(buf, len);
	Write(s, buf, len);

	printf("\nReply returned:\n");
	/* Read and print SADB_ADD reply, discarding any others */
	for (;;) {
		int msglen;
		struct sadb_msg *msgp;

		msglen = Read(s, &buf, sizeof(buf));
		msgp = (struct sadb_msg *)&buf;
		if (msgp->sadb_msg_pid == mypid &&
			msgp->sadb_msg_type == SADB_ADD) {
			print_sadb_msg(msgp, msglen);
			break;
		}
	}
	close(s);
}
/* end sadb_add */

int
main(int argc, char **argv)
{
	struct addrinfo hints, *src, *dst;
	unsigned char *p, *keydata, *kp;
	char *ep;
	int ret, len, i;
	int satype, alg, keybits;

	bzero(&hints, sizeof(hints));
	if ((ret = getaddrinfo(argv[1], NULL, &hints, &src)) != 0) {
		err_quit("%s: %s\n", argv[1], gai_strerror(ret));
	}
	if ((ret = getaddrinfo(argv[2], NULL, &hints, &dst)) != 0) {
		err_quit("%s: %s\n", argv[2], gai_strerror(ret));
	}
	if (src->ai_family != dst->ai_family) {
		err_quit("%s and %s not same addr family\n", argv[1], argv[2]);
	}
	satype = SADB_SATYPE_AH;
	if ((alg = getsaalgbyname(satype, argv[3])) < 0) {
		err_quit("Unknown SA type / algorithm pair ah/%s\n", argv[3]);
	}
	keybits = strtoul(argv[4], &ep, 0);
	if (ep == argv[4] || *ep != '\0' || (keybits % 8) != 0) {
		err_quit("Invalid number of bits %s\n", argv[4]);
	}
	p = argv[5];
	if (p[0] == '0' && (p[1] == 'x' || p[1] == 'X'))
		p += 2;
	len = strlen(p);
	kp = keydata = malloc(keybits / 8);
	for (i = 0; i < keybits; i += 8) {
		int c;

		if (len < 2) {
			err_quit("%s: not enough bytes (expected %d)\n", argv[5], keybits / 8);
		}
		if (sscanf(p, "%2x", &c) != 1) {
			err_quit("%s contains invalid hex digit\n", argv[5]);
		}
		*kp++ = c;
		p += 2;
		len -= 2;
	}
	if (len > 0) {
		err_quit("%s: too many bytes (expected %d)\n", argv[5], keybits / 8);
	}
	sadb_add(src->ai_addr, dst->ai_addr, satype, alg, 0x9876, keybits, keydata);
}
