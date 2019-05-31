include ../Make.defines

PROGS =	udpcli01 udpserv01 udpcli02 udpcli03 udpcli04 \
		udpcli06 udpserv06 udpserv07 udpcli08 udpcli09 udpcli10 \
		udpservselect01

all:	${PROGS}

udpcli01:	udpcli01.o
		${CC} ${CFLAGS} -o $@ udpcli01.o ${LIBS}

udpserv01:	udpserv01.o
		${CC} ${CFLAGS} -o $@ udpserv01.o ${LIBS}

udpcli02:	udpcli02.o dgcliaddr.o
		${CC} ${CFLAGS} -o $@ udpcli02.o dgcliaddr.o ${LIBS}

udpcli03:	udpcli03.o dgcliinetaddr.o
		${CC} ${CFLAGS} -o $@ udpcli03.o dgcliinetaddr.o ${LIBS}

udpcli04:	udpcli04.o dgcliconnect.o
		${CC} ${CFLAGS} -o $@ udpcli04.o dgcliconnect.o ${LIBS}

udpcli06:	udpcli06.o dgcliloop1.o
		${CC} ${CFLAGS} -o $@ udpcli06.o dgcliloop1.o ${LIBS}

udpserv06:	udpserv06.o dgecholoop1.o
		${CC} ${CFLAGS} -o $@ udpserv06.o dgecholoop1.o ${LIBS}

udpcli07:	udpcli07.o dgcliloop2.o
		${CC} ${CFLAGS} -o $@ udpcli07.o dgcliloop2.o ${LIBS}

udpserv07:	udpserv07.o dgecholoop2.o
		${CC} ${CFLAGS} -o $@ udpserv07.o dgecholoop2.o ${LIBS}

udpcli08:	udpcli08.o dgcliloop3.o
		${CC} ${CFLAGS} -o $@ udpcli08.o dgcliloop3.o ${LIBS}

udpcli09:	udpcli09.o
		${CC} ${CFLAGS} -o $@ udpcli09.o ${LIBS}

udpcli10:	udpcli10.o dgclibig.o
		${CC} ${CFLAGS} -o $@ udpcli10.o dgclibig.o ${LIBS}

udpservselect01:	udpservselect01.o sigchldwaitpid.o
		${CC} ${CFLAGS} -o $@ udpservselect01.o sigchldwaitpid.o ${LIBS}

clean:
		rm -f ${PROGS} ${CLEANFILES}
