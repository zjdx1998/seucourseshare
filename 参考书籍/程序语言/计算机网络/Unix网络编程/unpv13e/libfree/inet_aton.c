/*
 *  inet_aton.c,v 1.3 1993/05/19 03:39:32 jch Exp
 */

/* Gated Release 3.5 */
/* Copyright (c) 1990,1991,1992,1993,1994,1995 by Cornell University.  All */
/* rights reserved.  Refer to Particulars and other Copyright notices at */
/* the end of this file.  */
/*  */

#include	<sys/types.h>
#include	<netinet/in.h>

/* 
 * Check whether "cp" is a valid ascii representation
 * of an Internet address and convert to a binary address.
 * Returns 1 if the address is valid, 0 if not.
 * This replaces inet_addr, the return value from which
 * cannot distinguish between failure and a local broadcast address.
 */

int
inet_aton(const char *cp, struct in_addr *ap)
{
    int dots = 0;
    register u_long acc = 0, addr = 0;

    do {
	register char cc = *cp;

	switch (cc) {
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':
	    acc = acc * 10 + (cc - '0');
	    break;

	case '.':
	    if (++dots > 3) {
		return 0;
	    }
	    /* Fall through */

	case '\0':
	    if (acc > 255) {
		return 0;
	    }
	    addr = addr << 8 | acc;
	    acc = 0;
	    break;

	default:
	    return 0;
	}
    } while (*cp++) ;

    /* Normalize the address */
    if (dots < 3) {
	addr <<= 8 * (3 - dots) ;
    }

    /* Store it if requested */
    if (ap) {
	ap->s_addr = htonl(addr);
    }

    return 1;    
}

/*
 * ------------------------------------------------------------------------
 * 
 * 	GateD, Release 3.5
 * 
 * 	Copyright (c) 1990,1991,1992,1993,1994,1995 by Cornell University.
 * 	    All rights reserved.
 * 
 * 	THIS SOFTWARE IS PROVIDED "AS IS" AND WITHOUT ANY
 * 	EXPRESS OR IMPLIED WARRANTIES, INCLUDING, WITHOUT
 * 	LIMITATION, THE IMPLIED WARRANTIES OF MERCHANTABILITY
 * 	AND FITNESS FOR A PARTICULAR PURPOSE.
 * 
 * 	Royalty-free licenses to redistribute GateD Release
 * 	3 in whole or in part may be obtained by writing to:
 * 
 * 	    GateDaemon Project
 * 	    Information Technologies/Network Resources
 * 	    200 CCC
 * 	    Cornell University
 * 	    Ithaca, NY  14853-2601  USA
 * 
 * 	GateD is based on Kirton's EGP, UC Berkeley's routing
 * 	daemon	 (routed), and DCN's HELLO routing Protocol.
 * 	Development of GateD has been supported in part by the
 * 	National Science Foundation.
 * 
 * 	Please forward bug fixes, enhancements and questions to the
 * 	gated mailing list: gated-people@gated.cornell.edu.
 * 
 * ------------------------------------------------------------------------
 * 
 *       Portions of this software may fall under the following
 *       copyrights:
 * 
 * 	Copyright (c) 1988 Regents of the University of California.
 * 	All rights reserved.
 * 
 * 	Redistribution and use in source and binary forms are
 * 	permitted provided that the above copyright notice and
 * 	this paragraph are duplicated in all such forms and that
 * 	any documentation, advertising materials, and other
 * 	materials related to such distribution and use
 * 	acknowledge that the software was developed by the
 * 	University of California, Berkeley.  The name of the
 * 	University may not be used to endorse or promote
 * 	products derived from this software without specific
 * 	prior written permission.  THIS SOFTWARE IS PROVIDED
 * 	``AS IS'' AND WITHOUT ANY EXPRESS OR IMPLIED WARRANTIES,
 * 	INCLUDING, WITHOUT LIMITATION, THE IMPLIED WARRANTIES OF
 * 	MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 */
