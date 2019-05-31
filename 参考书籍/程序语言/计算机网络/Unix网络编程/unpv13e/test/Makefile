include ../Make.defines

PROGS =	accept_eintr test1 treadline1 treadline2 treadline3 \
		tsnprintf tisfdtype tshutdown

TEST1_OBJS = test1.o funcs.o

all:	${PROGS}

test1:	${TEST1_OBJS}
		${CC} ${CFLAGS} -o $@ ${TEST1_OBJS} ${LIBS}

test2:	test2.o
		${CC} ${CFLAGS} -o $@ test2.o ${LIBS}

accept_eintr:	accept_eintr.o
		${CC} ${CFLAGS} -o $@ accept_eintr.o ${LIBS}

treadline1:	treadline1.o readline1.o
		${CC} ${CFLAGS} -o $@ treadline1.o readline1.o ${LIBS}

treadline2:	treadline2.o readline2.o
		${CC} ${CFLAGS} -o $@ treadline2.o readline2.o ${LIBS}

treadline3:	treadline3.o readline3.o
		${CC} ${CFLAGS} -o $@ treadline3.o readline3.o ${LIBS}

tsnprintf:	tsnprintf.o
		${CC} ${CFLAGS} -o $@ tsnprintf.o ${LIBS}

tisfdtype:	tisfdtype.o
		${CC} ${CFLAGS} -o $@ tisfdtype.o ${LIBS}

tshutdown:	tshutdown.o
		${CC} ${CFLAGS} -o $@ tshutdown.o ${LIBS}

clean:
		rm -f ${PROGS} core core.* *.o temp.* *.out typescript*
