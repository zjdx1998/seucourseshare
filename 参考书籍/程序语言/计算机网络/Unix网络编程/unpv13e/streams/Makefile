include ../Make.defines

PROGS =	strlist_sock strlist_xti tpi_daytime

all:	${PROGS}

strlist_sock:	strlist_sock.o
		${CC} ${CFLAGS} -o $@ strlist_sock.o ${LIBS_XTI}

strlist_xti:	strlist_xti.o
		${CC} ${CFLAGS} -o $@ strlist_xti.o ${LIBS_XTI}

tpi_daytime:	tpi_daytime.o tpi_bind.o tpi_connect.o tpi_read.o tpi_close.o
		${CC} ${CFLAGS} -o $@ tpi_daytime.o tpi_bind.o tpi_connect.o \
			tpi_read.o tpi_close.o ${LIBS_XTI}

clean:
		rm -f ${PROGS} ${CLEANFILES}
