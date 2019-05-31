include ../Make.defines

OBJS = main.o icmpcode_v4.o icmpcode_v6.o recv_v4.o recv_v6.o \
		sig_alrm.o traceloop.o tv_sub.o
PROGS =	traceroute

all:	${PROGS}

traceroute:	${OBJS}
		${CC} ${CFLAGS} -o $@ ${OBJS} ${LIBS}

clean:
		rm -f ${PROGS} ${CLEANFILES}
