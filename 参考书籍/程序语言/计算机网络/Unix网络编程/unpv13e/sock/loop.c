/*
 * Copyright (c) 1993 W. Richard Stevens.  All rights reserved.
 * Permission to use or modify this software and its documentation only for
 * educational purposes and without fee is hereby granted, provided that
 * the above copyright notice appear in all copies.  The author makes no
 * representations about the suitability of this software for any purpose.
 * It is provided "as is" without express or implied warranty.
 */

#include	"sock.h"

/* Copy everything from stdin to "sockfd",
 * and everything from "sockfd" to stdout. */

void	tty_atexit(void);	/* in library */
void	sig_catch(int);		/* my function */

void
loop(int sockfd)
{
	int						maxfdp1, nread, ntowrite, stdineof, clilen;
	fd_set					rset;
	struct sockaddr_in		cliaddr;		/* for UDP server */

#ifdef	MSG_TRUNC			/* 4.3BSD Reno and later */
	struct iovec			iov[1];
	struct msghdr			msg;

#ifdef	IP_RECVDSTADDR		/* 4.3BSD Reno and later */
	static struct cmsghdr  *cmptr = NULL;	/* malloc'ed */
	struct in_addr			dstinaddr;		/* for UDP server */
#define	CONTROLLEN	(sizeof(struct cmsghdr) + sizeof(struct in_addr))
#endif	/* IP_RECVDSTADDR */

#endif	/* MSG_TRUNC */

#ifdef	notdef	/* following doesn't appear to work */
	/*
	 * This is an attempt to set stdin to cbreak, so that input characters
	 * are delivered one at a time, to see Nagle algorithm in effect
	 * (or disabled).
	 */
	if (cbreak && isatty(STDIN_FILENO)) {
		if (tty_cbreak(STDIN_FILENO) < 0)
			err_sys("tty_cbreak error");
		if (atexit(tty_atexit) < 0)
			err_sys("tty_atexit error");

		if (signal(SIGINT, sig_catch) == SIG_ERR)
			err_sys("signal error");
		if (signal(SIGQUIT, sig_catch) == SIG_ERR)
			err_sys("signal error");
		if (signal(SIGTERM, sig_catch) == SIG_ERR)
			err_sys("signal error");
	}
#endif

	if (pauseinit)
		sleep(pauseinit);	/* intended for server */

	stdineof = 0;
	FD_ZERO(&rset);
	maxfdp1 = sockfd + 1;	/* check descriptors [0..sockfd] */

		/* UDP client issues connect(), so read() and write() are used.
		   Server is harder since cannot issue connect().  We use recvfrom()
		   or recvmsg(), depending on OS. */

	for ( ; ; ) {
		if (stdineof == 0)
			FD_SET(STDIN_FILENO, &rset);
		FD_SET(sockfd, &rset);

		if (select(maxfdp1, &rset, NULL, NULL, NULL) < 0)
			err_sys("select error");

		if (FD_ISSET(STDIN_FILENO, &rset)) {	/* data to read on stdin */
			if ( (nread = read(STDIN_FILENO, rbuf, readlen)) < 0)
				err_sys("read error from stdin");
			else if (nread == 0) {	/* EOF on stdin */
				if (halfclose) {
					if (shutdown(sockfd, 1) < 0)
						err_sys("shutdown() error");

					FD_CLR(STDIN_FILENO, &rset);
					stdineof = 1;	/* don't read stdin anymore */
					continue;		/* back to select() */
				}
				break;		/* default: stdin EOF -> done */
			}

			if (crlf) {
				ntowrite = crlf_add(wbuf, writelen, rbuf, nread);
				if (write(sockfd, wbuf, ntowrite) != ntowrite)
					err_sys("write error");
			} else {
				if (write(sockfd, rbuf, nread) != nread)
					err_sys("write error");
			}
		}

		if (FD_ISSET(sockfd, &rset)) {	/* data to read from socket */
			if (udp && server) {
				clilen = sizeof(cliaddr);
#ifndef	MSG_TRUNC	/* vanilla BSD sockets */
				nread = recvfrom(sockfd, rbuf, readlen, 0,
									(struct sockaddr *) &cliaddr, &clilen);

#else	/* 4.3BSD Reno and later; use recvmsg() to get at MSG_TRUNC flag */
		/* Also lets us get at control information (destination address) */


				iov[0].iov_base = rbuf;
				iov[0].iov_len  = readlen;
				msg.msg_iov          = iov;
				msg.msg_iovlen       = 1;
				msg.msg_name         = (caddr_t) &cliaddr;
				msg.msg_namelen      = clilen;

#ifdef	IP_RECVDSTADDR
				if (cmptr == NULL && (cmptr = malloc(CONTROLLEN)) == NULL)
					err_sys("malloc error for control buffer");

				msg.msg_control      = (caddr_t) cmptr;	/* for dest address */
				msg.msg_controllen   = CONTROLLEN;
#else
				msg.msg_control      = (caddr_t) 0;	/* no ancillary data */
				msg.msg_controllen   = 0;
#endif	/* IP_RECVDSTADDR */
				msg.msg_flags        = 0;			/* flags returned here */

				nread = recvmsg(sockfd, &msg, 0);
#endif	/* MSG_TRUNC */
				if (nread < 0)
					err_sys("datagram receive error");

				if (verbose) {
					printf("from %s", INET_NTOA(cliaddr.sin_addr));
#ifdef	MSG_TRUNC
#ifdef	IP_RECVDSTADDR
					if (recvdstaddr) {
						if (cmptr->cmsg_level != IPPROTO_IP)
							err_quit("control level != IPPROTO_IP");
						if (cmptr->cmsg_type != IP_RECVDSTADDR)
							err_quit("control type != IP_RECVDSTADDR");
						if (cmptr->cmsg_len != CONTROLLEN)
							err_quit("control length (%d) != %d",
									 cmptr->cmsg_len, CONTROLLEN);
						memcpy((char *) &dstinaddr, (char *) CMSG_DATA(cmptr),
							  sizeof(struct in_addr));

						printf(", to %s", INET_NTOA(dstinaddr));
					}
#endif	/* IP_RECVDSTADDR */
#endif	/* MSG_TRUNC */
					printf(": ");
					fflush(stdout);
				}

#ifdef	MSG_TRUNC
				if (msg.msg_flags & MSG_TRUNC)
					printf("(datagram truncated)\n");
#endif

			} else {
				if ( (nread = read(sockfd, rbuf, readlen)) < 0)
					err_sys("read error");
				else if (nread == 0) {
					if (verbose)
						fprintf(stderr, "connection closed by peer\n");
					break;		/* EOF, terminate */
				}
			}

			if (crlf) {
				ntowrite = crlf_strip(wbuf, writelen, rbuf, nread);
				if (writen(STDOUT_FILENO, wbuf, ntowrite) != ntowrite)
					err_sys("writen error to stdout");
			} else {
				if (writen(STDOUT_FILENO, rbuf, nread) != nread)
					err_sys("writen error to stdout");
			}
		}
	}

	if (pauseclose) {
		if (verbose)
				fprintf(stderr, "pausing before close\n");
		sleep(pauseclose);
	}

	if (close(sockfd) < 0)
		err_sys("close error");		/* since SO_LINGER may be set */
}

void
sig_catch(int signo)
{
	exit(0);	/* exit handler will reset tty state */
}
