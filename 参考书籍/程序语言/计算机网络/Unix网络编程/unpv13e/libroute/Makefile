include ../Make.defines

all:	${LIBROUTE_OBJS}
		ar rv ${LIBUNP_NAME} $?
		${RANLIB} ${LIBUNP_NAME}

clean:
		rm -f ${PROGS} ${CLEANFILES}
