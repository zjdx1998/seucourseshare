include ../Make.defines

OBJS = main.o cleanup.o pcap.o udpcksum.o senddnsquery-raw.o udpread.o udpwrite.o
OBJSNET = main.o cleanup.o pcap.o udpcksum.o senddnsquery-libnet.o udpread.o
PROGS =	udpcksum udpcksum-libnet

all:	${PROGS}

udpcksum:	${OBJS}
		${CC} ${CFLAGS} -o $@ ${OBJS} -L/usr/local/lib -lpcap ${LIBS}

# Include special linking flags from libnet-config program
udpcksum-libnet:	${OBJSNET}
		${CC} ${CFLAGS} -o $@ ${OBJSNET} -L/usr/local/lib -lpcap ${LIBS} `libnet-config --libs`

# Include special compilation flags from libnet-config program
senddnsquery-libnet.o:	senddnsquery-libnet.c
		${CC} ${CFLAGS} `libnet-config --defines --cflags` -c -o $@ $<

clean:
		rm -f ${PROGS} ${CLEANFILES}
