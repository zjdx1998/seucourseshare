include ../Make.defines

PROGS =	daytimetcpcli daytimetcpsrv2 mycat openfile \
	tfcred01 unixbind unixstrcli01 unixstrserv01 unixstrserv02

all:	${PROGS}

daytimetcpcli:	daytimetcpcli.o
		${CC} ${CFLAGS} -o $@ daytimetcpcli.o ${LIBS}

daytimetcpsrv2:	daytimetcpsrv2.o
		${CC} ${CFLAGS} -o $@ daytimetcpsrv2.o ${LIBS}

mycat:		mycat.o myopen.o
		${CC} ${CFLAGS} -o $@ mycat.o myopen.o ${LIBS}

openfile:	openfile.o
		${CC} ${CFLAGS} -o $@ openfile.o ${LIBS}

tfcred01:	tfcred01.o
		${CC} ${CFLAGS} -o $@ tfcred01.o ${LIBS}

unixbind:	unixbind.o
		${CC} ${CFLAGS} -o $@ unixbind.o ${LIBS}

unixdgcli01:	unixdgcli01.o
		${CC} ${CFLAGS} -o $@ unixdgcli01.o ${LIBS}

unixdgserv01:	unixdgserv01.o
		${CC} ${CFLAGS} -o $@ unixdgserv01.o ${LIBS}

unixstrcli01:	unixstrcli01.o
		${CC} ${CFLAGS} -o $@ unixstrcli01.o ${LIBS}

unixstrserv01:	unixstrserv01.o sigchldwaitpid.o
		${CC} ${CFLAGS} -o $@ unixstrserv01.o sigchldwaitpid.o ${LIBS}

unixstrserv02:	unixstrserv02.o strecho.o sigchldwaitpid.o readcred.o
		${CC} ${CFLAGS} -o $@ unixstrserv02.o strecho.o sigchldwaitpid.o \
				readcred.o ${LIBS}

clean:
		rm -f ${PROGS} ${CLEANFILES}
