include ../Make.defines

PROGS =	sock
OBJS = buffers.o cliopen.o crlf.o error.o looptcp.o loopudp.o \
	   main.o multicast.o pattern.o servopen.o sleepus.o sockopts.o \
	   sourceroute.o sourcetcp.o sourceudp.o sinktcp.o sinkudp.o \
	   tellwait.o write.o

all:	${PROGS}

${OBJS}: sock.h

sock:	${OBJS}
		${CC} ${CFLAGS} -o $@ ${OBJS} ${LIBS}

clean:
		rm -f ${PROGS} core core.* *.o temp.* *.out typescript*
