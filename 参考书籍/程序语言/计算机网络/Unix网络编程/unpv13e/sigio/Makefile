include ../Make.defines

PROGS =	udpcli01 udpserv01

all:	${PROGS}

udpcli01:	udpcli01.o dgcli01.o
		${CC} ${CFLAGS} -o $@ udpcli01.o dgcli01.o ${LIBS}

udpserv01:	udpserv01.o dgecho01.o
		${CC} ${CFLAGS} -o $@ udpserv01.o dgecho01.o ${LIBS}

clean:
		rm -f ${PROGS} ${CLEANFILES}
