include ../Make.defines

PROGS =	tcpcli01 tcpserv01

all:	${PROGS}

tcpcli01:	tcpcli01.o sourceroute.o
		${CC} ${CFLAGS} -o $@ tcpcli01.o sourceroute.o ${LIBS}

tcpserv01:	tcpserv01.o sourceroute.o sigchldwaitpid.o
		${CC} ${CFLAGS} -o $@ tcpserv01.o sourceroute.o sigchldwaitpid.o \
				${LIBS}

udpcli01:	udpcli01.o 
		${CC} ${CFLAGS} -o $@ udpcli01.o ${LIBS}

udpserv01:	udpserv01.o sourceroute6.o dgechoprintroute.o
		${CC} ${CFLAGS} -o $@ udpserv01.o sourceroute6.o dgechoprintroute.o ${LIBS}


clean:
		rm -f ${PROGS} ${CLEANFILES}
