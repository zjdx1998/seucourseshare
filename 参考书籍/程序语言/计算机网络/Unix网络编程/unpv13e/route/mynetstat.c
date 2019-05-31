#include	"unproute.h"

void	pr_rtable(int);
void	pr_iflist(int);

int
main(int argc, char **argv)
{
	int family;

	if (argc != 2)
		err_quit("usage: mynetstat <inet4|inet6|all>");
	if (strcmp(argv[1], "inet4") == 0)
		family = AF_INET;
#ifdef	AF_INET6
	else if (strcmp(argv[1], "inet6") == 0)
		family = AF_INET6;
#endif
	else if (strcmp(argv[1], "all") == 0)
		family = 0;
	else
		err_quit("invalid <address-family>");

	pr_rtable(family);

	pr_iflist(family);

	exit(0);
}

void
pr_rtable(int family)
{
	char				*buf, *next, *lim;
	size_t				len;
	struct rt_msghdr	*rtm;
	struct sockaddr		*sa, *rti_info[RTAX_MAX];

	buf = Net_rt_dump(family, 0, &len);

	lim = buf + len;
	for (next = buf; next < lim; next += rtm->rtm_msglen) {
		rtm = (struct rt_msghdr *) next;
		sa = (struct sockaddr *)(rtm + 1);
		get_rtaddrs(rtm->rtm_addrs, sa, rti_info);
		if ( (sa = rti_info[RTAX_DST]) != NULL)
			printf("dest: %s", sock_ntop(sa, sa->sa_len));

		if ( (sa = rti_info[RTAX_GATEWAY]) != NULL)
			printf(", gateway: %s", sock_ntop(sa, sa->sa_len));

		printf("\n");
	}
}

void
pr_iflist(int family)
{
	int 				flags;
	char				*buf, *next, *lim;
	u_char				*ptr;
	size_t				len;
	struct if_msghdr	*ifm;
	struct ifa_msghdr	*ifam;
	struct sockaddr		*sa, *rti_info[RTAX_MAX];
	struct sockaddr_dl	*sdl;

	buf = Net_rt_iflist(family, 0, &len);

	lim = buf + len;
	for (next = buf; next < lim; next += ifm->ifm_msglen) {
		ifm = (struct if_msghdr *) next;
		if (ifm->ifm_type == RTM_IFINFO) {
			sa = (struct sockaddr *)(ifm + 1);
			get_rtaddrs(ifm->ifm_addrs, sa, rti_info);
			if ( (sa = rti_info[RTAX_IFP]) != NULL) {
				if (((flags = ifm->ifm_flags) & IFF_UP) == 0)
					continue;
				printf("interface: %s: <", Sock_ntop(sa, sa->sa_len));
				if (flags & IFF_UP)				printf("UP ");
				if (flags & IFF_BROADCAST)		printf("BCAST ");
				if (flags & IFF_MULTICAST)		printf("MCAST ");
				if (flags & IFF_LOOPBACK)		printf("LOOP ");
				if (flags & IFF_POINTOPOINT)	printf("P2P ");
				printf(">\n");

				if (sa->sa_family == AF_LINK &&
					(sdl = (struct sockaddr_dl *) sa) &&
					(sdl->sdl_alen > 0)) {
					ptr = (u_char *) &sdl->sdl_data[sdl->sdl_nlen];
					printf("  %x:%x:%x:%x:%x:%x\n", *ptr, *(ptr+1),
							*(ptr+2), *(ptr+3), *(ptr+4), *(ptr+5));
				}
			}

		} else if (ifm->ifm_type == RTM_NEWADDR) {
			ifam = (struct ifa_msghdr *) next;
			sa = (struct sockaddr *)(ifam + 1);
			get_rtaddrs(ifam->ifam_addrs, sa, rti_info);
			if ( (sa = rti_info[RTAX_IFA]) != NULL)
				printf("  IP addr: %s\n", Sock_ntop(sa, sa->sa_len));
			if ((flags & IFF_BROADCAST) && (sa = rti_info[RTAX_BRD]))
				printf("  bcast addr: %s\n", Sock_ntop(sa, sa->sa_len));

		} else
			err_quit("unexpected message type %d", ifm->ifm_type);
	}
}
