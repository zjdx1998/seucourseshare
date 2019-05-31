QUICK AND DIRTY
===============

Execute the following from the src/ directory:

    ./configure    # try to figure out all implementation differences

    cd lib         # build the basic library that all programs need
    make           # use "gmake" everywhere on BSD/OS systems

    cd ../libfree  # continue building the basic library
    make

    cd ../libroute # only if your system supports 4.4BSD style routing sockets
    make           # only if your system supports 4.4BSD style routing sockets

    cd ../libxti   # only if your system supports XTI
    make           # only if your system supports XTI

    cd ../intro    # build and test a basic client program
    make daytimetcpcli
    ./daytimetcpcli 127.0.0.1

If all that works, you're all set to start compiling individual programs.

Notice that all the source code assumes tabs every 4 columns, not 8.

MORE DETAILS
============

5.  If you need to make any changes to the "unp.h" header, notice that it
    is a hard link in each directory, so you only need to change it once.

6.  Go into the "lib/" directory and type "make".  This builds the library
    "libunp.a" that is required by almost all of the programs.  There may
    be compiler warnings (see NOTES below).  This step is where you'll find
    all of your system's dependencies, and you must just update your cf/
    files from step 1, rerun "config" and do this step again.

6.  Go into the "libfree/" directory and type "make".  This adds to the
    "libunp.a" library.  The files in this directory do not #include
    the "unp.h" header, as people may want to use these functions
    independent of the book's examples.

8.  Once the library is made from steps 5 and 6, you can then go into any
    of the source code directories and make whatever program you are
    interested in.  Note that the horizontal rules at the beginning and
    end of each program listing in the book contain the directory name and
    filename.

    BEWARE: Not all programs in each directory will compile on all systems
    (e.g., the file src/advio/recvfromflags.c will not compile unless your
    system supports the IP_RECVDSTADDR socket option).  Also, not all files
    in each directory are included in the book.  Beware of any files with
    "test" in the filename: they are probably a quick test program that I
    wrote to check something, and may or may not work.

NOTES
-----

- Many systems do not have correct function prototypes for the socket
  functions, and this can cause many warnings during compilation.
  For example, Solaris 2.5 omits the "const" from the 2nd argument
  to connect().  Lots of systems use "int" for the length of socket
  address structures, while Posix.1g specifies "size_t".  Lots of
  systems still have the pointer argument to [sg]etsockopt() as a
  "char *" instead of a "void *", and this also causes warnings.

- SunOS 4.1.x: If you are using Sun's acc compiler, you need to run
  the configure program as

        CC=acc CFLAGS=-w CPPFLAGS=-w ./configure

  Failure to do this results in numerous system headers (<sys/sockio.h>)
  not being found during configuration, causing compile errors later.

- If your system supports IPv6 and you want to run the examples in the
  book using hostnames, you must install the latest BIND release.  You
  can get it from ftp://ftp.vix.com/pub/bind/release.  All you need from
  this release is a resolver library that you should then add to the
  LDLIBS and LDLIBS_THREADS lines.

- IPv6 support is still in its infancy.  There may be differences
  between the IPv6 sockets API specifications and what the vendor
  provides.  This may require hand tweaking, but should get better
  over time.

- If your system supports an older draft of the Posix pthreads standard,
  but configure detects the support of pthreads, you will have to disable
  this by hand.  Digital Unix V3.2C has this problem, for example, as it
  supports draft 4, not the final draft.

  To fix this, remove wrappthread.o from LIB_OBJS in "Make.defines" and
  don't try to build and run any of the threads programs.

COMMON DIFFERENCES
------------------

These are the common differences that I see in various headers that are
not "yet" at the level of Posix.1g or X/Open XNS Issue 5.

- getsockopt() and setsockopt(): 5th argument is not correct type.

- t_bind(): second argument is missing "const".

- t_connect(): second argument is missing "const".

- t_open(): first argument is missing "const".

- t_optmsmg(): second argument is missing "const".

- If your <xti.h> defines the members of the t_opthdr{} as longs,
  instead of t_uscalar_t, some of the printf formats of these value
  might generate warnings from your compiler, since you are printing
  a long without a corresponding long format specifier.
