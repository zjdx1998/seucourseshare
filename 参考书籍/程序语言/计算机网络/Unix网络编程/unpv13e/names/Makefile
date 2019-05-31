include ../Make.defines

PROGS =	daytimetcpcli daytimetcpcli1 daytimetcpsrv1 daytimetcpsrv2 \
		daytimetcpsrv3 daytimetcpsrv4 \
		daytimeudpcli1 daytimetcpcli2 daytimetcpcli3 \
		daytimeudpcli2 daytimeudpsrv2 daytimeudpsrv3 \
		hostent hostent2 hostent3 \
		netent prmyaddrs prmyaddrs1 test1

all:	${PROGS}

daytimetcpcli:	daytimetcpcli.o
		${CC} ${CFLAGS} -o $@ daytimetcpcli.o ${LIBS}

daytimetcpcli1:	daytimetcpcli1.o
		${CC} ${CFLAGS} -o $@ daytimetcpcli1.o ${LIBS}

daytimetcpcli2:	daytimetcpcli2.o
		${CC} ${CFLAGS} -o $@ daytimetcpcli2.o ${LIBS}

daytimetcpcli3:	daytimetcpcli3.o
		${CC} ${CFLAGS} -o $@ daytimetcpcli3.o ${LIBS}

daytimetcpsrv1:	daytimetcpsrv1.o
		${CC} ${CFLAGS} -o $@ daytimetcpsrv1.o ${LIBS}

daytimetcpsrv2:	daytimetcpsrv2.o
		${CC} ${CFLAGS} -o $@ daytimetcpsrv2.o ${LIBS}

daytimetcpsrv3:	daytimetcpsrv3.o
		${CC} ${CFLAGS} -o $@ daytimetcpsrv3.o ${LIBS}

daytimetcpsrv4:	daytimetcpsrv4.o
		${CC} ${CFLAGS} -o $@ daytimetcpsrv4.o ${LIBS}

daytimeudpcli1:	daytimeudpcli1.o
		${CC} ${CFLAGS} -o $@ daytimeudpcli1.o ${LIBS}

daytimeudpcli2:	daytimeudpcli2.o
		${CC} ${CFLAGS} -o $@ daytimeudpcli2.o ${LIBS}

daytimeudpsrv2:	daytimeudpsrv2.o
		${CC} ${CFLAGS} -o $@ daytimeudpsrv2.o ${LIBS}

daytimeudpsrv3:	daytimeudpsrv3.o udp_server_reuseaddr.o
		${CC} ${CFLAGS} -o $@ daytimeudpsrv3.o udp_server_reuseaddr.o \
			${LIBS}

hostent:	hostent.o
		${CC} ${CFLAGS} -o $@ hostent.o ${LIBS}

hostent2:	hostent2.o
		${CC} ${CFLAGS} -o $@ hostent2.o ${LIBS}

hostent3:	hostent3.o
		${CC} ${CFLAGS} -o $@ hostent3.o ${LIBS}

netent:	netent.o
		${CC} ${CFLAGS} -o $@ netent.o ${LIBS}

prmyaddrs:	prmyaddrs.o
		${CC} ${CFLAGS} -o $@ prmyaddrs.o ${LIBS}

prmyaddrs1:	prmyaddrs1.o myaddrs1.o
		${CC} ${CFLAGS} -o $@ prmyaddrs1.o myaddrs1.o ${LIBS}

test1:	test1.o
		${CC} ${CFLAGS} -o $@ test1.o ${LIBS}

test2:	test2.o
		${CC} ${CFLAGS} -o $@ test2.o ${LIBS}

clean:
		rm -f ${PROGS} ${CLEANFILES}
