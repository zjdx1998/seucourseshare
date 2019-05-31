include ../Make.defines

PROGS =	qlen backlog test01 test02 test03 test04 test05 test06

all:	${PROGS}

backlog:	backlog.o
		${CC} ${CFLAGS} -o $@ backlog.o ${LIBS}

qlen:	qlen.o
		${CC} ${CFLAGS} -o $@ qlen.o ${LIBS_XTI}

test01:	test01.o
		${CC} ${CFLAGS} -o $@ test01.o ${LIBS_XTI}

test02:	test02.o
		${CC} ${CFLAGS} -o $@ test02.o ${LIBS}

test03:	test03.o
		${CC} ${CFLAGS} -o $@ test03.o ${LIBS_XTI}

test04:	test04.o
		${CC} ${CFLAGS} -o $@ test04.o ${LIBS}

test05:	test05.o
		${CC} ${CFLAGS} -o $@ test05.o ${LIBS_XTI}

test06:	test06.o
		${CC} ${CFLAGS} -o $@ test06.o ${LIBS_XTI}

clean:
		rm -f ${PROGS} ${CLEANFILES}
