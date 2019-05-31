#ifndef	__unpicmp_h
#define	__unpicmp_h

#include	"unp.h"

#define	ICMPD_PATH		"/tmp/icmpd"	/* server's well-known pathname */

struct icmpd_err {
  int				icmpd_errno;/* EHOSTUNREACH, EMSGSIZE, ECONNREFUSED */
  char				icmpd_type;	/* actual ICMPv[46] type */
  char				icmpd_code;	/* actual ICMPv[46] code */
  socklen_t			icmpd_len;	/* length of sockaddr{} that follows */
  struct sockaddr_storage	icmpd_dest;	/* sockaddr_storage handles any size */
};

#endif	/* __unpicmp_h */
