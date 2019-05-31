include ../Make.defines

PROGS =	udpcli01 udpcli02 udpcli03 udpcli04 udpcli05 udpcli06

all:	${PROGS}

# Version in book.
udpcli01:	udpcli01.o dgclibcast1.o
		${CC} ${CFLAGS} -o $@ udpcli01.o dgclibcast1.o ${LIBS}

# Incorrect version with alarm(1) and sleep(1) to tickle race condition.
udpcli02:	udpcli02.o dgclibcast2.o
		${CC} ${CFLAGS} -o $@ udpcli02.o dgclibcast2.o ${LIBS}

# Incorrect version that blocks signals, but still has a race condition.
udpcli03:	udpcli03.o dgclibcast3.o
		${CC} ${CFLAGS} -o $@ udpcli03.o dgclibcast3.o ${LIBS}

# Correct version using pselect().
udpcli04:	udpcli04.o dgclibcast4.o
		${CC} ${CFLAGS} -o $@ udpcli04.o dgclibcast4.o ${LIBS}

# Correct version using sigsetjmp()/siglongjmp().
udpcli05:	udpcli05.o dgclibcast5.o
		${CC} ${CFLAGS} -o $@ udpcli05.o dgclibcast5.o ${LIBS}

# Correct version using a pipe from signal handler to select().
udpcli06:	udpcli06.o dgclibcast6.o
		${CC} ${CFLAGS} -o $@ udpcli06.o dgclibcast6.o ${LIBS}

clean:
		rm -f ${PROGS} ${CLEANFILES}
