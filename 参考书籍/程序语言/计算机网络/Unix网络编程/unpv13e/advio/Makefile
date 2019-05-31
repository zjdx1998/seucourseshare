include ../Make.defines

PROGS =	tcpcli01 tcpcli02 tcpserv02 \
		udpcli01 udpcli02 udpcli03 \
		udpserv01 udpserv03 udpserv04 \
		daytimetcpcli daytimeudpcli3 daytimeudpcli4

all:	${PROGS}

tcpcli01:	tcpcli01.o
		${CC} ${CFLAGS} -o $@ tcpcli01.o ${LIBS}

tcpcli02:	tcpcli02.o str_cli_select02.o
		${CC} ${CFLAGS} -o $@ tcpcli02.o str_cli_select02.o ${LIBS}

tcpcli03:	tcpcli03.o str_cli_poll03.o
		${CC} ${CFLAGS} -o $@ tcpcli03.o str_cli_poll03.o ${LIBS}

tcpcli04:	tcpcli04.o str_cli_kqueue04.o
		${CC} ${CFLAGS} -o $@ tcpcli04.o str_cli_kqueue04.o ${LIBS}

tcpserv02:	tcpserv02.o str_echo_stdio02.o sig_chld_waitpid.o
		${CC} ${CFLAGS} -o $@ tcpserv02.o str_echo_stdio02.o \
			sig_chld_waitpid.o ${LIBS}

udpcli01:	udpcli01.o dgclitimeo1.o
		${CC} ${CFLAGS} -o $@ udpcli01.o dgclitimeo1.o ${LIBS}

udpcli02:	udpcli02.o dgclitimeo2.o
		${CC} ${CFLAGS} -o $@ udpcli02.o dgclitimeo2.o ${LIBS}

udpcli03:	udpcli03.o dgclitimeo3.o
		${CC} ${CFLAGS} -o $@ udpcli03.o dgclitimeo3.o ${LIBS}

udpserv01:	udpserv01.o dgechoaddr.o recvfromflags.o
		${CC} ${CFLAGS} -o $@ udpserv01.o dgechoaddr.o recvfromflags.o ${LIBS}

udpserv03:	udpserv03.o
		${CC} ${CFLAGS} -o $@ udpserv03.o ${LIBS}

udpserv04:	udpserv04.o
		${CC} ${CFLAGS} -o $@ udpserv04.o ${LIBS}

daytimetcpcli:	daytimetcpcli.o
		${CC} ${CFLAGS} -o $@ daytimetcpcli.o ${LIBS}

daytimeudpcli3:	daytimeudpcli3.o
		${CC} ${CFLAGS} -o $@ daytimeudpcli3.o ${LIBS}

daytimeudpcli4:	daytimeudpcli4.o
		${CC} ${CFLAGS} -o $@ daytimeudpcli4.o ${LIBS}

clean:
		rm -f ${PROGS} ${CLEANFILES}
