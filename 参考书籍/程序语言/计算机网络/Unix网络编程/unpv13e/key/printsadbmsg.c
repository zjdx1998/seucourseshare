#include "unp.h"
#include <net/pfkeyv2.h>

const char *
get_sadb_msg_type(int type)
{
	static char buf[100];
	switch (type) {
	case SADB_RESERVED:	return "Reserved";
	case SADB_GETSPI:	return "Get SPI";
	case SADB_UPDATE:	return "Update";
	case SADB_ADD:		return "Add";
	case SADB_DELETE:	return "Delete";
	case SADB_GET:		return "Get";
	case SADB_ACQUIRE:	return "Acquire";
	case SADB_REGISTER:	return "Register";
	case SADB_EXPIRE:	return "Expire";
	case SADB_FLUSH:	return "Flush";
	case SADB_DUMP:		return "Dump";
	default:			sprintf(buf, "[Unknown type %d]", type);
						return buf;
	}
}

const char *
get_sadb_satype(int type)
{
	static char buf[100];
	switch (type) {
	case SADB_SATYPE_UNSPEC:	return "Unspecified";
	case SADB_SATYPE_AH:		return "IPsec AH";
	case SADB_SATYPE_ESP:		return "IPsec ESP";
	case SADB_SATYPE_RSVP:		return "RSVP";
	case SADB_SATYPE_OSPFV2:	return "OSPFv2";
	case SADB_SATYPE_RIPV2:		return "RIPv2";
	case SADB_SATYPE_MIP:		return "Mobile IP";
	default:					sprintf(buf, "[Unknown satype %d]", type);
								return buf;
	}
}

const char *
get_auth_alg(int alg)
{
	static char buf[100];
	switch (alg) {
	case SADB_AALG_NONE:		return "None";
	case SADB_AALG_MD5HMAC:		return "HMAC-MD5";
	case SADB_AALG_SHA1HMAC:	return "HMAC-SHA-1";
#ifdef SADB_X_AALG_MD5
	case SADB_X_AALG_MD5:		return "Keyed MD5";
#endif
#ifdef SADB_X_AALG_SHA
	case SADB_X_AALG_SHA:		return "Keyed SHA-1";
#endif
#ifdef SADB_X_AALG_NULL
	case SADB_X_AALG_NULL:		return "Null";
#endif
#ifdef SADB_X_AALG_SHA2_256
	case SADB_X_AALG_SHA2_256:	return "SHA2-256";
#endif
#ifdef SADB_X_AALG_SHA2_384
	case SADB_X_AALG_SHA2_384:	return "SHA2-384";
#endif
#ifdef SADB_X_AALG_SHA2_512
	case SADB_X_AALG_SHA2_512:	return "SHA2-512";
#endif
	default:					sprintf(buf, "[Unknown authentication algorithm %d]", alg);
								return buf;
	}
}

const char *
get_encrypt_alg(int alg)
{
	static char buf[100];
	switch (alg) {
	case SADB_EALG_NONE:		return "None";
	case SADB_EALG_DESCBC:		return "DES-CBC";
	case SADB_EALG_3DESCBC:		return "3DES-CBC";
	case SADB_EALG_NULL:		return "Null";
#ifdef SADB_X_EALG_CAST128CBC
	case SADB_X_EALG_CAST128CBC:	return "CAST128-CBC";
#endif
#ifdef SADB_X_EALG_BLOWFISHCBC
	case SADB_X_EALG_BLOWFISHCBC:	return "Blowfish-CBC";
#endif
#ifdef SADB_X_EALG_AES
	case SADB_X_EALG_AES:			return "AES";
#endif
	default:					sprintf(buf, "[Unknown encryption algorithm %d]", alg);
								return buf;
	}
}

const char *
get_sa_state(int state)
{
	static char buf[100];
	switch (state) {
	case SADB_SASTATE_LARVAL:	return "Larval";
	case SADB_SASTATE_MATURE:	return "Mature";
	case SADB_SASTATE_DYING:	return "Dying";
	case SADB_SASTATE_DEAD:		return "Dead";
	default:					sprintf(buf, "[Unknown SA state %d]", state);
								return buf;
	}
}

const char *
get_sadb_alg_type(int alg, int authenc)
{
	if (authenc == SADB_EXT_SUPPORTED_AUTH) {
		return get_auth_alg(alg);
	} else {
		return get_encrypt_alg(alg);
	}
}

void
sa_print(struct sadb_ext *ext)
{
	struct sadb_sa *sa = (struct sadb_sa *)ext;
	printf(" SA: SPI=%d Replay Window=%d State=%s\n",
		sa->sadb_sa_spi, sa->sadb_sa_replay,
		get_sa_state(sa->sadb_sa_state));
	printf("  Authentication Algorithm: %s\n",
		get_auth_alg(sa->sadb_sa_auth));
	printf("  Encryption Algorithm: %s\n",
		get_encrypt_alg(sa->sadb_sa_encrypt));
	if (sa->sadb_sa_flags & SADB_SAFLAGS_PFS)
		printf("  Perfect Forward Secrecy\n");
}

void
supported_print(struct sadb_ext *ext)
{
	struct sadb_supported *sup = (struct sadb_supported *)ext;
	struct sadb_alg *alg;
	int len;

	printf(" Supported %s algorithms:\n",
		sup->sadb_supported_exttype == SADB_EXT_SUPPORTED_AUTH ?
		"authentication" :
		"encryption");
	len = sup->sadb_supported_len * 8;
	len -= sizeof(*sup);
	if (len == 0) {
		printf("  None\n");
		return;
	}
	for (alg = (struct sadb_alg *)(sup + 1); len>0; len -= sizeof(*alg), alg++) {
		printf("  %s ivlen %d bits %d-%d\n",
			get_sadb_alg_type(alg->sadb_alg_id, sup->sadb_supported_exttype),
			alg->sadb_alg_ivlen, alg->sadb_alg_minbits, alg->sadb_alg_maxbits);
	}
}

void
lifetime_print(struct sadb_ext *ext)
{
	struct sadb_lifetime *life = (struct sadb_lifetime *)ext;

	printf(" %s lifetime:\n",
		life->sadb_lifetime_exttype == SADB_EXT_LIFETIME_CURRENT ?
		"Current" :
		life->sadb_lifetime_exttype == SADB_EXT_LIFETIME_HARD ?
		"Hard" :
		"Soft");
	printf("  %d allocations, %d bytes", life->sadb_lifetime_allocations,
		life->sadb_lifetime_bytes);
	if (life->sadb_lifetime_exttype == SADB_EXT_LIFETIME_CURRENT) {
		time_t t;
		struct tmp *tm;
		char buf[100];

		/* absolute times */
		t = life->sadb_lifetime_addtime;
		tm = localtime(&t);
		strftime(buf, sizeof(buf), "%c", tm);
		printf("\n  added at %s, ", buf);
		if (life->sadb_lifetime_usetime == 0) {
			printf("never used\n");
		} else {
			t = life->sadb_lifetime_usetime;
			tm = localtime(&t);
			strftime(buf, sizeof(buf), "%c", tm);
			printf("first used at %s\n", buf);
		}
	} else {
		printf("%d addtime, %d usetime\n", life->sadb_lifetime_addtime,
			life->sadb_lifetime_usetime);
	}
}

void
address_print(struct sadb_ext *ext)
{
	struct sadb_address *addr = (struct sadb_address *)ext;
	struct sockaddr *sa;

	printf(" %s address: ",
		addr->sadb_address_exttype == SADB_EXT_ADDRESS_SRC ?
		"Source" :
		addr->sadb_address_exttype == SADB_EXT_ADDRESS_DST ?
		"Dest" :
		"Proxy");
	sa = (struct sockaddr *)(addr + 1);
	printf("  %s", sock_ntop(sa, addr->sadb_address_len * 8 - sizeof(*addr)));
	if (addr->sadb_address_prefixlen == 0)
		printf(" ");
	else
		printf("/%d ", addr->sadb_address_prefixlen);
	switch (addr->sadb_address_proto) {
		case IPPROTO_UDP:	printf("(UDP)"); break;
		case IPPROTO_TCP:	printf("(TCP)"); break;
		case 0:				break;
		default:			printf("(IP proto %d)", addr->sadb_address_proto);
							break;
	}
	printf("\n");
}

void
key_print(struct sadb_ext *ext)
{
	struct sadb_key *key = (struct sadb_key *)ext;
	int bits;
	unsigned char *p;

	printf(" %s key, %d bits: 0x",
		key->sadb_key_exttype == SADB_EXT_KEY_AUTH ?
		"Authentication" : "Encryption",
		key->sadb_key_bits);
	for (p = (unsigned char *)(key + 1), bits = key->sadb_key_bits;
			bits > 0; p++, bits -= 8)
		printf("%02x", *p);
	printf("\n");
}

void
print_sadb_msg(struct sadb_msg *msg, int msglen)
{
	struct sadb_ext *ext;

	if (msglen != msg->sadb_msg_len * 8) {
		err_msg("SADB Message length (%d) doesn't match msglen (%d)\n",
			msg->sadb_msg_len * 8, msglen);
		return;
	}
	if (msg->sadb_msg_version != PF_KEY_V2) {
		err_msg("SADB Message version not PF_KEY_V2\n");
		return;
	}
	printf("SADB Message %s, errno %d, satype %s, seq %d, pid %d\n",
		get_sadb_msg_type(msg->sadb_msg_type), msg->sadb_msg_errno,
		get_sadb_satype(msg->sadb_msg_satype), msg->sadb_msg_seq,
		msg->sadb_msg_pid);
	if (msg->sadb_msg_errno != 0)
		printf(" errno %s\n", strerror(msg->sadb_msg_errno));
	if (msglen == sizeof(struct sadb_msg))
		return;	/* no extensions */
	msglen -= sizeof(struct sadb_msg);
	ext = (struct sadb_ext *)(msg + 1);
	while (msglen > 0) {
		switch (ext->sadb_ext_type) {
		case SADB_EXT_RESERVED:	printf(" Reserved Extension\n"); break;
		case SADB_EXT_SA:	sa_print(ext); break;
		case SADB_EXT_LIFETIME_CURRENT:
		case SADB_EXT_LIFETIME_HARD:
		case SADB_EXT_LIFETIME_SOFT:
					lifetime_print(ext); break;
		case SADB_EXT_ADDRESS_SRC:
		case SADB_EXT_ADDRESS_DST:
		case SADB_EXT_ADDRESS_PROXY:
					address_print(ext); break;
		case SADB_EXT_KEY_AUTH:
		case SADB_EXT_KEY_ENCRYPT:
					key_print(ext); break;
		case SADB_EXT_IDENTITY_SRC:
		case SADB_EXT_IDENTITY_DST:
					printf(" [identity...]\n"); break;
		case SADB_EXT_SENSITIVITY:
					printf(" [sensitivity...]\n"); break;
		case SADB_EXT_PROPOSAL:
					printf(" [proposal...]\n"); break;
		case SADB_EXT_SUPPORTED_AUTH:
		case SADB_EXT_SUPPORTED_ENCRYPT:
					supported_print(ext); break;
		case SADB_EXT_SPIRANGE:
					printf(" [spirange...]\n"); break;
		default:	printf(" [unknown extension %d]\n", ext->sadb_ext_type);
		}
		msglen -= ext->sadb_ext_len << 3;
		ext = (char *)ext + (ext->sadb_ext_len << 3);
	}
}
