/* include unpxtih1 */
#ifndef	__unp_xti_h
#define	__unp_xti_h

#include	"unp.h"

#include	<xti.h>
#ifdef	HAVE_XTI_INET_H
# include	<xti_inet.h>
#endif
#ifdef	HAVE_NETCONFIG_H
# include	<netconfig.h>
#endif
#ifdef	HAVE_NETDIR_H
# include	<netdir.h>
#endif

#ifdef	INFTIM_UNPH
#undef	INFTIM	/* was not in <poll.h>, undef for <stropts.h> */
#endif

#include	<stropts.h>

/* Provide compatibility with the new names prepended with T_
   in XNS Issue 5, which are not in Posix.1g. */

#ifndef	T_INET_TCP
#define	T_INET_TCP		INET_TCP
/* $$.Ic T_INET_TCP$$ */
#endif
/* end unpxtih1 */
#ifndef	T_INET_UDP
#define	T_INET_UDP		INET_UDP
#endif
#ifndef	T_INET_IP
#define	T_INET_IP		INET_IP
#endif
#ifndef	T_TCP_NODELAY
#define	T_TCP_NODELAY	TCP_NODELAY
#endif
#ifndef	T_TCP_MAXSEG
#define	T_TCP_MAXSEG	TCP_MAXSEG
#endif
#ifndef	T_TCP_KEEPALIVE
#define	T_TCP_KEEPALIVE	TCP_KEEPALIVE
#endif
#ifndef	T_UDP_CHECKSUM
#define	T_UDP_CHECKSUM	UDP_CHECKSUM
#endif
#ifndef	T_IP_OPTIONS
#define	T_IP_OPTIONS	IP_OPTIONS
#endif
#ifndef	T_IP_TOS
#define	T_IP_TOS		IP_TOS
#endif
#ifndef	T_IP_TTL
#define	T_IP_TTL		IP_TTL
#endif
#ifndef	T_IP_REUSEADDR
#define	T_IP_REUSEADDR	IP_REUSEADDR
#endif
#ifndef	T_IP_DONTROUTE
#define	T_IP_DONTROUTE	IP_DONTROUTE
#endif
/* include unpxtih2 */
#ifndef	T_IP_BROADCAST
#define	T_IP_BROADCAST	IP_BROADCAST
/* $$.Ic T_IP_BROADCAST$$ */
#endif

/* Define the appropriate devices for t_open(). */
#ifdef	HAVE_DEV_TCP
# define	XTI_TCP		"/dev/tcp"
# define	XTI_UDP		"/dev/udp"
#endif
#ifdef	HAVE_DEV_XTI_TCP
# define	XTI_TCP		"/dev/xti/tcp"
# define	XTI_UDP		"/dev/xti/udp"
#endif
#ifdef	HAVE_DEV_STREAMS_XTISO_TCP
# define	XTI_TCP		"/dev/streams/xtiso/tcp+"	/* + for XPG4 */
# define	XTI_UDP		"/dev/streams/xtiso/udp+"	/* + for XPG4 */
#endif

	/* 4device to t_open() for t_accept(); set by tcp_listen() */
/* $$.Id xti_serv_dev$$ */
extern char	xti_serv_dev[];
/* end unpxtih2 */

void	 err_xti(const char *fmt, ...);
void	 err_xti_ret(const char *fmt, ...);

int		 Getmsg(int, struct strbuf *, struct strbuf *, int *);
void	 Putmsg(int, const struct strbuf *, const struct strbuf *, int);

#ifdef	HAVE_NETCONFIG_H
void	*Setnetconfig(void);
void	*Setnetpath(void);
#endif

void	*T_alloc(int, int, int);
int		 T_accept(int, int, struct t_call *);
void	 T_bind(int, const struct t_bind *, struct t_bind *);
void	 T_close(int);
void	 T_connect(int, const struct t_call *, struct t_call *);
void	 T_free(void *, int);
void	 T_getprotaddr(int, struct t_bind *, struct t_bind *);
int		 T_getstate(int);
void	 T_listen(int, struct t_call *);
int		 T_look(int);
int		 T_open(const char *, int, struct t_info *);
void	 T_optmgmt(int, const struct t_optmgmt *, struct t_optmgmt *);
int		 T_rcv(int, void *, unsigned int, int *);
void	 T_rcvdis(int, struct t_discon *);
void	 T_rcvrel(int);
void	 T_rcvudata(int, struct t_unitdata *, int *);
void	 T_rcvuderr(int, struct t_uderr *);
void	 T_snd(int, void *, unsigned int, int);
void	 T_sndrel(int);
void	 T_sndudata(int, struct t_unitdata *);

int		 xti_accept(int, struct netbuf *, int);
int		 xti_getopt(int, int, int, void *, socklen_t *);
char	*xti_flags_str(int);
char	*xti_tlook_str(int);
char	*xti_ntop(const struct netbuf *);
char	*xti_ntop_host(const struct netbuf *);
int		 xti_rdwr(int);
int		 xti_setopt(int, int, int, void *, socklen_t);

int		 Xti_accept(int, struct netbuf *, int);
void	 Xti_getopt(int, int, int, void *, socklen_t *);
char	*Xti_flags_str(int);
char	*Xti_tlook_str(int);
char	*Xti_ntop(const struct netbuf *);
char	*Xti_ntop_host(const struct netbuf *);
void	 Xti_rdwr(int);
void	 Xti_setopt(int, int, int, void *, socklen_t);

char	*xti_str_lend(struct t_opthdr *);
char	*xti_str_uscalard(struct t_opthdr *);
char	*xti_str_uchard(struct t_opthdr *);
char	*xti_str_ucharx(struct t_opthdr *);
char	*xti_str_yn(t_uscalar_t);
char	*xti_str_syng(t_scalar_t);
char	*xti_str_uiyn(struct t_opthdr *);
char	*xti_str_usyn(struct t_opthdr *);
char	*xti_str_linger(struct t_opthdr *);
char	*xti_str_kpalive(struct t_opthdr *);
char	*xti_str_flags(t_scalar_t);

#endif	/* __unp_xti_h */
