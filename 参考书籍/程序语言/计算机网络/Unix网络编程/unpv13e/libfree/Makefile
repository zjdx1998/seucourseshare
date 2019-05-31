include ../Make.defines

all:	${LIBFREE_OBJS}
		ar rv ${LIBUNP_NAME} $?
		${RANLIB} ${LIBUNP_NAME}

test_inet_pton:	test_inet_pton.o
		${CC} ${CFLAGS} -o $@ test_inet_pton.o ${LIBS}

clean:
		rm -f ${PROGS} ${CLEANFILES}
