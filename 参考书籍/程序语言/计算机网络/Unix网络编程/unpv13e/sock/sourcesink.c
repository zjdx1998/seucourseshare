/*
 * Copyright (c) 1993 W. Richard Stevens.  All rights reserved.
 * Permission to use or modify this software and its documentation only for
 * educational purposes and without fee is hereby granted, provided that
 * the above copyright notice appear in all copies.  The author makes no
 * representations about the suitability of this software for any purpose.
 * It is provided "as is" without express or implied warranty.
 */

#include	"sock.h"
#include	<ctype.h>

void	pattern(char *ptr, int len);

void
sink(int sockfd)
{
	int		i, n;
	char	oob;

	if (client) {
		pattern(wbuf, writelen);	/* fill send buffer with a pattern */

		if (pauseinit)
			sleep(pauseinit);

		for (i = 1; i <= nbuf; i++) {
			if (urgwrite == i) {
				oob = urgwrite;
				if ( (n = send(sockfd, &oob, 1, MSG_OOB)) != 1)
					err_sys("send of MSG_OOB returned %d, expected %d",
																n, writelen);
				if (verbose)
					fprintf(stderr, "wrote %d byte of urgent data\n", n);
			}

			if ( (n = write(sockfd, wbuf, writelen)) != writelen)
				err_sys("write returned %d, expected %d", n, writelen);
			if (verbose)
				fprintf(stderr, "wrote %d bytes\n", n);

			if (pauserw)
				sleep(pauserw);
		}

	} else {

		if (pauseinit)
			sleep(pauseinit);

		for ( ; ; ) {
			if ( (n = read(sockfd, rbuf, readlen)) < 0) {
				err_sys("read error");

			} else if (n == 0) {
				break;	 	/* connection closed by peer */

			} else if (n != readlen)
				err_quit("read returned %d, expected %d", n, readlen);
	
			if (verbose)
				fprintf(stderr, "received %d bytes\n", n);

			if (pauserw)
				sleep(pauserw);
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
pattern(char *ptr, int len)
{
	char	c;

	c = 0;
	while(len-- > 0)  {
		while(isprint((c & 0x7F)) == 0)
			c++;	/* skip over nonprinting characters */
		*ptr++ = (c++ & 0x7F);
	}
}
