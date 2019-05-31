include ../Make.defines

PROGS =	daytimetcpsrv2 daytimetcpsrv3

all:	${PROGS}

daytimetcpsrv2:	daytimetcpsrv2.o
		${CC} ${CFLAGS} -o $@ daytimetcpsrv2.o ${LIBS}

daytimetcpsrv3:	daytimetcpsrv3.o
		${CC} ${CFLAGS} -o $@ daytimetcpsrv3.o ${LIBS}

clean:
		rm -f ${PROGS} ${CLEANFILES}
