cc=gcc

all: thread-create thread-create2 thread-create2-revised primes detached

thread-create: thread-create.c
	${cc} -pthread -o thread-create thread-create.c

thread-create2: thread-create2.c
	${cc} -pthread -o thread-create2 thread-create2.c

thread-create2-revised: thread-create2-revised.c
	${cc} -pthread -o thread-create2-revised thread-create2-revised.c

primes: primes.c
	${cc} -pthread -o primes primes.c

detached: detached.c
	${cc} -pthread -o detached detached.c
