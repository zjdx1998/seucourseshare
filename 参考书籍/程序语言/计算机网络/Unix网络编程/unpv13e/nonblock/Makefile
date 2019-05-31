include ../Make.defines

PROGS =	daytimetcpcli tcpcli01 tcpcli02 tcpcli03 tcpcli04 tcpservselect02 web

all:	${PROGS}

daytimetcpcli:	daytimetcpcli.o
		${CC} ${CFLAGS} -o $@ daytimetcpcli.o ${LIBS}

tcpcli01:	tcpcli01.o strclifork.o
		${CC} ${CFLAGS} -o $@ tcpcli01.o strclifork.o ${LIBS}

tcpcli02:	tcpcli02.o strclinonb.o
		${CC} ${CFLAGS} -o $@ tcpcli02.o strclinonb.o ${LIBS}

tcpcli03:	tcpcli03.o
		${CC} ${CFLAGS} -o $@ tcpcli03.o ${LIBS}

tcpcli04:	tcpcli04.o
		${CC} ${CFLAGS} -o $@ tcpcli04.o ${LIBS}

tcpservselect03:	tcpservselect03.o
		${CC} ${CFLAGS} -o $@ tcpservselect03.o ${LIBS}

web:	web.o home_page.o start_connect.o write_get_cmd.o
		${CC} ${CFLAGS} -o $@ web.o home_page.o start_connect.o \
			write_get_cmd.o ${LIBS}

clean:
		rm -f ${PROGS} ${CLEANFILES}
