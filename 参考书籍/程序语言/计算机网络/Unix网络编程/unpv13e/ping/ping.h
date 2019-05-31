#include	"unp.h"
#include	<netinet/in_systm.h>
#include	<netinet/ip.h>
#include	<netinet/ip_icmp.h>

#define	BUFSIZE		1500

			/* globals */
char	 sendbuf[BUFSIZE];

int		 datalen;			/* # bytes of data following ICMP header */
char	*host;
int		 nsent;				/* add 1 for each sendto() */
pid_t	 pid;				/* our PID */
int		 sockfd;
int		 verbose;

			/* function prototypes */
void	 init_v6(void);
void	 proc_v4(char *, ssize_t, struct msghdr *, struct timeval *);
void	 proc_v6(char *, ssize_t, struct msghdr *, struct timeval *);
void	 send_v4(void);
void	 send_v6(void);
void	 readloop(void);
void	 sig_alrm(int);
void	 tv_sub(struct timeval *, struct timeval *);

struct proto {
  void	 (*fproc)(char *, ssize_t, struct msghdr *, struct timeval *);
  void	 (*fsend)(void);
  void	 (*finit)(void);
  struct sockaddr  *sasend;	/* sockaddr{} for send, from getaddrinfo */
  struct sockaddr  *sarecv;	/* sockaddr{} for receiving */
  socklen_t	    salen;		/* length of sockaddr{}s */
  int	   	    icmpproto;	/* IPPROTO_xxx value for ICMP */
} *pr;

#ifdef	IPV6

#include	<netinet/ip6.h>
#include	<netinet/icmp6.h>

#endif
