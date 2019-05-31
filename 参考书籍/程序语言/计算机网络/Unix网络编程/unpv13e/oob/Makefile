include ../Make.defines

# XXX get autoconf to put tcprecv03p in here if the system supports poll
PROGS =	tcprecv01 tcprecv02 tcprecv03 tcprecv04 tcprecv05 tcprecv06 \
	 tcpsend01 tcpsend02 tcpsend03 tcpsend04 tcpsend05 tcpsend06 \
	 tcpcli02 tcpserv02

all:	${PROGS}

tcprecv01:	tcprecv01.o
		${CC} ${CFLAGS} -o $@ tcprecv01.o ${LIBS}

tcpsend01:	tcpsend01.o
		${CC} ${CFLAGS} -o $@ tcpsend01.o ${LIBS}

tcprecv02:	tcprecv02.o
		${CC} ${CFLAGS} -o $@ tcprecv02.o ${LIBS}

tcpsend02:	tcpsend02.o
		${CC} ${CFLAGS} -o $@ tcpsend02.o ${LIBS}

tcprecv03:	tcprecv03.o
		${CC} ${CFLAGS} -o $@ tcprecv03.o ${LIBS}

tcprecv03p:	tcprecv03p.o
		${CC} ${CFLAGS} -o $@ tcprecv03p.o ${LIBS}

tcpsend03:	tcpsend03.o
		${CC} ${CFLAGS} -o $@ tcpsend03.o ${LIBS}

tcprecv04:	tcprecv04.o
		${CC} ${CFLAGS} -o $@ tcprecv04.o ${LIBS}

tcpsend04:	tcpsend04.o
		${CC} ${CFLAGS} -o $@ tcpsend04.o ${LIBS}

tcprecv05:	tcprecv05.o
		${CC} ${CFLAGS} -o $@ tcprecv05.o ${LIBS}

tcpsend05:	tcpsend05.o
		${CC} ${CFLAGS} -o $@ tcpsend05.o ${LIBS}

tcprecv06:	tcprecv06.o
		${CC} ${CFLAGS} -o $@ tcprecv06.o ${LIBS}

tcpsend06:	tcpsend06.o
		${CC} ${CFLAGS} -o $@ tcpsend06.o ${LIBS}

tcpcli02:	tcpcli02.o strcliselect02.o heartbeatcli.o
		${CC} ${CFLAGS} -o $@ tcpcli02.o strcliselect02.o heartbeatcli.o ${LIBS}

tcpserv02:	tcpserv02.o strecho02.o heartbeatserv.o sigchldwaitpid.o
		${CC} ${CFLAGS} -o $@ tcpserv02.o strecho02.o heartbeatserv.o sigchldwaitpid.o ${LIBS}

clean:
		rm -f ${PROGS} ${CLEANFILES}
