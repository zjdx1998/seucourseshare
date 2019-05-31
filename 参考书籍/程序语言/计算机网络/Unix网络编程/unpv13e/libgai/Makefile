include ../Make.defines

# Note: In the source code in this directory I #ifdef the constants
# IPv4, IPv6 (both lowercase "v"), and UNIXdomain.  This is instead of
# the all-uppercase constants so that these #ifdef/#endif lines do not
# appear in the book (too much clutter, given the amount of conditional
# testing for all the code in this directory).

all:	${LIBGAI_OBJS}
		ar rv ${LIBUNP_NAME} $?
		${RANLIB} ${LIBUNP_NAME}

PROGS = testga test1

testga:	testga.o
		${CC} ${CFLAGS} -o $@ testga.o ${LIBS}

test1:	test1.o
		${CC} ${CFLAGS} -o $@ test1.o ${LIBS}

clean:
		rm -f ${PROGS} ${CLEANFILES}
