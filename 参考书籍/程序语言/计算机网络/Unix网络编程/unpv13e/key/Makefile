include ../Make.defines

PROGS =	dump register add
OBJS = printsadbmsg.o name.o

all:	${PROGS}

dump:	dump.o ${OBJS}
		${CC} ${CFLAGS} -o $@ $@.o ${OBJS} ${LIBS}

register:	register.o ${OBJS}
		${CC} ${CFLAGS} -o $@ $@.o ${OBJS} ${LIBS}

add:	add.o ${OBJS}
	${CC} ${CFLAGS} -o $@ $@.o ${OBJS} ${LIBS}
