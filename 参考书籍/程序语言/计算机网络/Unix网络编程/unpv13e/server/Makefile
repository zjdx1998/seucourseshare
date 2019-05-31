include ../Make.defines

PROGS =	client clientrst \
		serv01 serv02 serv03 serv04 serv05 serv06 serv07 serv08

all:	${PROGS}

# The client to test the various servers.
client:	client.o pr_cpu_time.o
		${CC} ${CFLAGS} -o $@ client.o pr_cpu_time.o ${LIBS}

# A special client that sends an RST occasionally.
# Used to test the XTI server (should receive disconnect).
clientrst:	clientrst.o pr_cpu_time.o
		${CC} ${CFLAGS} -o $@ clientrst.o pr_cpu_time.o ${LIBS}

# serv00: traditional concurrent server: use as base level
serv00:	serv00.o web_child.o pr_cpu_time.o
		${CC} ${CFLAGS} -o $@ serv00.o web_child.o pr_cpu_time.o ${LIBS}

# serv01: one fork per client (traditional concurrent server).
serv01:	serv01.o web_child.o sig_chld_waitpid.o pr_cpu_time.o
		${CC} ${CFLAGS} -o $@ serv01.o web_child.o sig_chld_waitpid.o \
			pr_cpu_time.o ${LIBS}

# serv02: prefork, no locking; works on BSD-derived systems
#	but not on SVR4-derived systems.
serv02:	serv02.o child02.o web_child.o pr_cpu_time.o
		${CC} ${CFLAGS} -o $@ serv02.o child02.o web_child.o pr_cpu_time.o ${LIBS}

# serv02l: prefork, no locking, block in select instead of accept to see
#	select collisions; works on BSD-derived systems but not on SVR4.
serv02l:serv02.o child02l.o web_child.o pr_cpu_time.o
		${CC} ${CFLAGS} -o serv02l serv02.o child02l.o web_child.o \
			pr_cpu_time.o ${LIBS}

# serv02m: prefork, no locking; works on BSD-derived systems.
#	This version is "metered" to see #clients/child serviced.
serv02m:serv02m.o child02m.o web_child.o pr_cpu_time.o meter.o
		${CC} ${CFLAGS} -o serv02m serv02m.o child02m.o web_child.o \
			pr_cpu_time.o meter.o ${LIBS}

# serv03: prefork, file locking using fcntl().  Similar to Apache server.
serv03:	serv03.o child03.o lock_fcntl.o web_child.o pr_cpu_time.o
		${CC} ${CFLAGS} -o $@ serv03.o child03.o lock_fcntl.o web_child.o \
			pr_cpu_time.o ${LIBS}

# serv03m: prefork, file locking using fcntl(), metered.
serv03m:	serv03m.o child03m.o lock_fcntl.o web_child.o pr_cpu_time.o meter.o
		${CC} ${CFLAGS} -o $@ serv03m.o child03m.o lock_fcntl.o web_child.o \
			pr_cpu_time.o meter.o ${LIBS}

# serv04: prefork, file locking using pthread locking.
serv04:	serv04.o child04.o lock_pthread.o web_child.o pr_cpu_time.o
		${CC} ${CFLAGS} -o $@ serv04.o child04.o lock_pthread.o \
			web_child.o pr_cpu_time.o ${LIBS}

# serv05: prefork, descrptor passing to children.  Similar to NSCA server.
serv05:	serv05.o child05.o lock_fcntl.o web_child.o pr_cpu_time.o
		${CC} ${CFLAGS} -o $@ serv05.o child05.o lock_fcntl.o web_child.o \
			pr_cpu_time.o ${LIBS}

# Thread versions must call a reentrant version of readline().
# serv06: one thread per client.
serv06:	serv06.o web_child.o pr_cpu_time.o readline.o
		${CC} ${CFLAGS} -o $@ serv06.o web_child.o pr_cpu_time.o \
			readline.o ${LIBS}

# serv07: prethread with mutex locking around accept().
serv07:	serv07.o pthread07.o web_child.o pr_cpu_time.o readline.o
		${CC} ${CFLAGS} -o $@ serv07.o pthread07.o web_child.o pr_cpu_time.o \
			readline.o ${LIBS}

# serv08: prethread with only main thread doing accept().
serv08:	serv08.o pthread08.o web_child.o pr_cpu_time.o readline.o
		${CC} ${CFLAGS} -o $@ serv08.o pthread08.o web_child.o pr_cpu_time.o \
			readline.o ${LIBS}

# serv09: prethread with no locking around accept().
serv09:	serv09.o pthread09.o web_child.o pr_cpu_time.o readline.o
		${CC} ${CFLAGS} -o $@ serv09.o pthread09.o web_child.o pr_cpu_time.o \
			readline.o ${LIBS}

clean:
		rm -f ${PROGS} ${CLEANFILES}
