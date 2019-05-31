include ../Make.defines

PROGS =	checkudpsum getrt mynetstat \
		prifinfo prifindex prifname prifnameindex

all:	${PROGS}

checkudpsum:	checkudpsum.o
		${CC} ${CFLAGS} -o $@ checkudpsum.o ${LIBS}

getrt:	getrt.o
		${CC} ${CFLAGS} -o $@ getrt.o ${LIBS}

prifinfo:	prifinfo.o get_ifi_info.o
		${CC} ${CFLAGS} -o $@ prifinfo.o get_ifi_info.o ${LIBS}

prifindex:	prifindex.o
		${CC} ${CFLAGS} -o $@ prifindex.o ${LIBS}

prifname:	prifname.o
		${CC} ${CFLAGS} -o $@ prifname.o ${LIBS}

prifnameindex:	prifnameindex.o
		${CC} ${CFLAGS} -o $@ prifnameindex.o ${LIBS}

mynetstat:	mynetstat.o
		${CC} ${CFLAGS} -o $@ mynetstat.o ${LIBS}

clean:
		rm -f ${PROGS} ${CLEANFILES}
