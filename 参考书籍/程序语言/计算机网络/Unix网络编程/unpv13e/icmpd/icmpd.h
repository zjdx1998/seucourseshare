#include	"unpicmpd.h"

struct client {
  int	connfd;			/* Unix domain stream socket to client */
  int	family;			/* AF_INET or AF_INET6 */
  int	lport;			/* local port bound to client's UDP socket */
						/* network byte ordered */
} client[FD_SETSIZE];

					/* 4globals */
int				fd4, fd6, listenfd, maxi, maxfd, nready;
fd_set			rset, allset;
struct sockaddr_un	cliaddr;

			/* 4function prototypes */
int		 readable_conn(int);
int		 readable_listen(void);
int		 readable_v4(void);
int		 readable_v6(void);
