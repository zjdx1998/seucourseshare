include ../Make.defines

OBJS = init_v6.o main.o proc_v4.o proc_v6.o readloop.o \
		send_v4.o send_v6.o sig_alrm.o tv_sub.o
PROGS =	ping

all:	${PROGS}

ping:	${OBJS}
		${CC} ${CFLAGS} -o $@ ${OBJS} ${LIBS}

clean:
		rm -f ${PROGS} ${CLEANFILES}
