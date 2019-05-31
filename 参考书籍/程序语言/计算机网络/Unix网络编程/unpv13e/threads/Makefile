include ../Make.defines

PROGS =	web01 web02 web03 \
	tcpcli01 tcpcli02 tcpserv01 tcpserv02 \
	test01 example01 example02 example03

all:	${PROGS}

web01:	web01.o
		${CC} ${CFLAGS} -o $@ web01.o ${LIBS}

web02:	web02.o
		${CC} ${CFLAGS} -o $@ web02.o ${LIBS}

web03:	web03.o
		${CC} ${CFLAGS} -o $@ web03.o ${LIBS}

tcpcli01:	tcpcli01.o strclithread.o
		${CC} ${CFLAGS} -o $@ tcpcli01.o strclithread.o ${LIBS}

tcpcli02:	tcpcli02.o strclithread2.o
		${CC} ${CFLAGS} -o $@ tcpcli02.o strclithread2.o ${LIBS}

tcpserv01:	tcpserv01.o
		${CC} ${CFLAGS} -o $@ tcpserv01.o ${LIBS}

# Broken one that uses readline() from library.
tcpserv02:	tcpserv02.o
		${CC} ${CFLAGS} -o $@ tcpserv02.o ${LIBS}

# Correct one that uses thread-safe readline().
tcpserv02g:	tcpserv02.o readline.o
		${CC} ${CFLAGS} -o $@ tcpserv02.o readline.o ${LIBS}

test01:	test01.o
		${CC} ${CFLAGS} -o $@ test01.o ${LIBS}

test02:	test02.o
		${CC} ${CFLAGS} -o $@ test02.o ${LIBS}

test03:	test03.o
		${CC} ${CFLAGS} -o $@ test03.o ${LIBS}

# Bad version uses readline() from library.
test04b:	test04.o
		${CC} ${CFLAGS} -o $@ test04.o ${LIBS}

# Good version uses readline.c in this directory.
test04g:	test04.o readline.o
		${CC} ${CFLAGS} -o $@ test04.o readline.o ${LIBS}

test05:	test05.o
		${CC} ${CFLAGS} -o $@ test05.o ${LIBS}

example01:	example01.o
		${CC} ${CFLAGS} -o $@ example01.o ${LIBS}

example02:	example02.o
		${CC} ${CFLAGS} -o $@ example02.o ${LIBS}

example03:	example03.o
		${CC} ${CFLAGS} -o $@ example03.o ${LIBS}

clean:
		rm -f ${PROGS} ${CLEANFILES}
