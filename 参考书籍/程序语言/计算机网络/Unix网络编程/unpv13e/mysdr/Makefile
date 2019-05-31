include ../Make.defines

PROGS =	mysdr

all:	${PROGS}

mysdr:	main.o loop.o
		${CC} ${CFLAGS} -o $@ main.o loop.o ${LIBS}

clean:
		rm -f ${PROGS} ${CLEANFILES}
