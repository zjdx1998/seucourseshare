#include "unp.h"
#include <net/pfkeyv2.h>

struct idlist {
	int val;
	const char *name;
};

static struct idlist satype[] = {
	{ SADB_SATYPE_UNSPEC,	"unspec" },
	{ SADB_SATYPE_AH,		"ah" },
	{ SADB_SATYPE_ESP,		"esp" },
	{ SADB_SATYPE_RSVP,		"rsvp" },
	{ SADB_SATYPE_OSPFV2,	"ospfv2" },
	{ SADB_SATYPE_RIPV2,	"ripv2" },
	{ SADB_SATYPE_MIP,		"mip" },
	{ 0,					NULL } };

static struct idlist ahalg[] = {
	{ SADB_AALG_NONE,		"none" },
	{ SADB_AALG_MD5HMAC,	"HMAC-MD5-96" },
	{ SADB_AALG_SHA1HMAC,	"HMAC-SHA-1-96" },
	{ 0,					NULL } };

static struct idlist espalg[] = {
	{ SADB_EALG_NONE,		"none" },
	{ SADB_EALG_DESCBC,		"DES-CBC" },
	{ SADB_EALG_3DESCBC,	"3DES-CBC" },
	{ SADB_EALG_NULL,		"NULL" },
	{ 0,					NULL } };

int
idlistlookup(char *name, struct idlist *il)
{
	for (; il->name != NULL; il++) {
		if (strcmp(name, il->name) == 0)
			return il->val;
	}
	return -1;
}

int
getsatypebyname(char *name)
{
	return idlistlookup(name, satype);
}

int
getsaalgbyname(int type, char *name)
{
	switch (type) {
	case SADB_SATYPE_AH:
		return idlistlookup(name, ahalg);
	case SADB_SATYPE_ESP:
		return idlistlookup(name, espalg);
	default:
		return -1;
	}
}
