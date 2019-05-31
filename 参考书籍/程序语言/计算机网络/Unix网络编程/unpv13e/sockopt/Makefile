include ../Make.defines

PROGS =	checkopts prdefaults rcvbuf rcvbufset sockopt

all:	${PROGS}

sockopt:	sockopt.o
		${CC} ${CFLAGS} -o $@ sockopt.o ${LIBS}

checkopts:	checkopts.o
		${CC} ${CFLAGS} -o $@ checkopts.o ${LIBS}

prdefaults:	prdefaults.o
		${CC} ${CFLAGS} -o $@ prdefaults.o ${LIBS}

rcvbuf:	rcvbuf.o
		${CC} ${CFLAGS} -o $@ rcvbuf.o ${LIBS}

rcvbufset:	rcvbufset.o
		${CC} ${CFLAGS} -o $@ rcvbufset.o ${LIBS}

clean:
		rm -f ${PROGS} ${CLEANFILES}
