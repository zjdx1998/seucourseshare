include ../Make.defines

PROGS =	sendrecv udpcli05 udpcli06 udpserv01

all:	${PROGS}

sendrecv:	main.o send.o recv.o
		${CC} ${CFLAGS} -o $@ main.o send.o recv.o ${LIBS}

# Version in book.
udpcli01:	udpcli01.o dgclibcast1.o
		${CC} ${CFLAGS} -o $@ udpcli01.o dgclibcast1.o ${LIBS}

# Correct version using sigsetjmp()/siglongjmp().
udpcli05:	udpcli05.o dgclimcast5.o
		${CC} ${CFLAGS} -o $@ udpcli05.o dgclimcast5.o ${LIBS}

# Try to bind multicast address and send.
udpcli06:	udpcli06.o dgclimcast6.o
		${CC} ${CFLAGS} -o $@ udpcli06.o dgclimcast6.o ${LIBS}

udpserv01:	udpserv01.o
		${CC} ${CFLAGS} -o $@ udpserv01.o ${LIBS}

clean:
		rm -f ${PROGS} ${CLEANFILES}
