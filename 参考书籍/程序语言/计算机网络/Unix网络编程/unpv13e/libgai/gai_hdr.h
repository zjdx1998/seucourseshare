#include	"unp.h"
#include	<ctype.h>		/* isxdigit(), etc. */

		/* following internal flag cannot overlap with other AI_xxx flags */
#define	AI_CLONE	     4	/* clone this entry for other socket types */

struct search {
  const char	*host;	/* hostname or address string */
  int			family;	/* AF_xxx */
};

		/* 4function prototypes for our own internal functions */
int		ga_aistruct(struct addrinfo ***, const struct addrinfo *,
					const void *, int);
struct addrinfo		*ga_clone(struct addrinfo *);
int		ga_echeck(const char *, const char *, int, int, int, int);
int		ga_nsearch(const char *, const struct addrinfo *, struct search *);
int		ga_port(struct addrinfo *, int , int);
int		ga_serv(struct addrinfo *, const struct addrinfo *, const char *);
int		ga_unix(const char *, struct addrinfo *, struct addrinfo **);

int		gn_ipv46(char *, size_t, char *, size_t, void *, size_t,
				 int, int, int);
