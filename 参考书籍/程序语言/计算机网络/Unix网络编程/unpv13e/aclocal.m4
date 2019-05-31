dnl ##################################################################
dnl Our macro to check for a function prototype in a given header.
dnl
AC_DEFUN(AC_CHECK_FUNC_PROTO,
	[AC_CACHE_CHECK(for $1 function prototype in $2, ac_cv_have_$1_proto,
		AC_EGREP_HEADER($1, $2,
			ac_cv_have_$1_proto=yes,
			ac_cv_have_$1_proto=no))
	if test $ac_cv_have_$1_proto = yes ; then
    		ac_tr_func=HAVE_`echo $1 | tr 'abcdefghijklmnopqrstuvwxyz' 'ABCDEFGHIJKLMNOPQRSTUVWXYZ'`_PROTO
		AC_DEFINE_UNQUOTED($ac_tr_func)
	fi
])

dnl ##################################################################
dnl We cannot use the AC_CHECK_TYPE macros because AC_CHECK_TYPE
dnl #includes only <sys/types.h>, <stdlib.h>, and <stddef.h>.
dnl Unfortunately, many implementations today hide typedefs in wierd
dnl locations: Solaris 2.5.1 has uint8_t and uint32_t in <pthread.h>.
dnl SunOS 4.1.x has int8_t in <sys/bitypes.h>.
dnl So we define our own macro AC_UNP_CHECK_TYPE that does the same
dnl #includes as "unp.h", and then looks for the typedef.
dnl
dnl This macro should be invoked after all the header checks have been
dnl performed, since we #include "confdefs.h" below, and then use the
dnl HAVE_foo_H values that it can #define.
dnl
AC_DEFUN(AC_UNP_CHECK_TYPE,
	[AC_MSG_CHECKING(if $1 defined)
	AC_CACHE_VAL(ac_cv_type_$1,
		AC_TRY_COMPILE(
[
#include	"confdefs.h"	/* the header built by configure so far */
#ifdef	HAVE_SYS_TYPES_H
#  include	<sys/types.h>
#endif
#ifdef	HAVE_SYS_SOCKET_H
#  include	<sys/socket.h>
#endif
#ifdef	HAVE_SYS_TIME_H
#  include    <sys/time.h>
#endif
#ifdef	HAVE_NETINET_IN_H
#  include    <netinet/in.h>
#endif
#ifdef	HAVE_ARPA_INET_H
#  include    <arpa/inet.h>
#endif
#ifdef	HAVE_ERRNO_H
#  include    <errno.h>
#endif
#ifdef	HAVE_FCNTL_H
#  include    <fcntl.h>
#endif
#ifdef	HAVE_NETDB_H
#  include	<netdb.h>
#endif
#ifdef	HAVE_SIGNAL_H
#  include	<signal.h>
#endif
#ifdef	HAVE_STDIO_H
#  include	<stdio.h>
#endif
#ifdef	HAVE_STDLIB_H
#  include	<stdlib.h>
#endif
#ifdef	HAVE_STRING_H
#  include	<string.h>
#endif
#ifdef	HAVE_SYS_STAT_H
#  include	<sys/stat.h>
#endif
#ifdef	HAVE_SYS_UIO_H
#  include	<sys/uio.h>
#endif
#ifdef	HAVE_UNISTD_H
#  include	<unistd.h>
#endif
#ifdef	HAVE_SYS_WAIT_H
#  include	<sys/wait.h>
#endif
#ifdef	HAVE_SYS_UN_H
#  include	<sys/un.h>
#endif
#ifdef	HAVE_SYS_SELECT_H
# include   <sys/select.h>
#endif
#ifdef	HAVE_STRINGS_H
# include   <strings.h>
#endif
#ifdef	HAVE_SYS_IOCTL_H
# include   <sys/ioctl.h>
#endif
#ifdef	HAVE_SYS_FILIO_H
# include   <sys/filio.h>
#endif
#ifdef	HAVE_SYS_SOCKIO_H
# include   <sys/sockio.h>
#endif
#ifdef	HAVE_PTHREAD_H
#  include	<pthread.h>
#endif],
		[ $1 foo ],
		ac_cv_type_$1=yes,
		ac_cv_type_$1=no))
	AC_MSG_RESULT($ac_cv_type_$1)
	if test $ac_cv_type_$1 = no ; then
		AC_DEFINE($1, $2, $3)
	fi
])

dnl ##################################################################
dnl The following checks for any typedefs for XTI programs.
dnl We perform all the #includes that "libxti/unpxti.h" performs.
dnl
AC_DEFUN(AC_UNPXTI_CHECK_TYPE,
	[AC_MSG_CHECKING(if $1 defined)
	AC_CACHE_VAL(ac_cv_type_$1,
		AC_TRY_COMPILE(
[
#include	"confdefs.h"	/* the header built by configure so far */
#ifdef	HAVE_SYS_TYPES_H
#  include	<sys/types.h>
#endif
#ifdef	HAVE_SYS_SOCKET_H
#  include	<sys/socket.h>
#endif
#ifdef	HAVE_SYS_TIME_H
#  include    <sys/time.h>
#endif
#ifdef	HAVE_NETINET_IN_H
#  include    <netinet/in.h>
#endif
#ifdef	HAVE_ARPA_INET_H
#  include    <arpa/inet.h>
#endif
#ifdef	HAVE_ERRNO_H
#  include    <errno.h>
#endif
#ifdef	HAVE_FCNTL_H
#  include    <fcntl.h>
#endif
#ifdef	HAVE_NETDB_H
#  include	<netdb.h>
#endif
#ifdef	HAVE_SIGNAL_H
#  include	<signal.h>
#endif
#ifdef	HAVE_STDIO_H
#  include	<stdio.h>
#endif
#ifdef	HAVE_STDLIB_H
#  include	<stdlib.h>
#endif
#ifdef	HAVE_STRING_H
#  include	<string.h>
#endif
#ifdef	HAVE_SYS_STAT_H
#  include	<sys/stat.h>
#endif
#ifdef	HAVE_SYS_UIO_H
#  include	<sys/uio.h>
#endif
#ifdef	HAVE_UNISTD_H
#  include	<unistd.h>
#endif
#ifdef	HAVE_SYS_WAIT_H
#  include	<sys/wait.h>
#endif
#ifdef	HAVE_SYS_UN_H
#  include	<sys/un.h>
#endif
#ifdef	HAVE_SYS_SELECT_H
# include   <sys/select.h>
#endif
#ifdef	HAVE_STRINGS_H
# include   <strings.h>
#endif
#ifdef	HAVE_SYS_IOCTL_H
# include   <sys/ioctl.h>
#endif
#ifdef	HAVE_SYS_FILIO_H
# include   <sys/filio.h>
#endif
#ifdef	HAVE_SYS_SOCKIO_H
# include   <sys/sockio.h>
#endif
#ifdef	HAVE_PTHREAD_H
#  include	<pthread.h>
#endif
#ifdef	HAVE_POLL_H
#  include	<poll.h>
#endif
#ifdef	HAVE_XTI_H
#  include	<xti.h>
#endif
#ifdef	HAVE_NETCONFIG_H
#  include	<netconfig.h>
#endif
#ifdef	HAVE_NETDIR_H
#  include	<netdir.h>
#endif
#ifdef	HAVE_STROPTS_H
#  include	<stropts.h>
#endif],
		[ $1 foo ],
		ac_cv_type_$1=yes,
		ac_cv_type_$1=no))
	AC_MSG_RESULT($ac_cv_type_$1)
	if test $ac_cv_type_$1 = no ; then
		AC_DEFINE($1, $2, $3)
	fi
])
