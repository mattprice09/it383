cc=gcc

all: thread-create thread-create2 thread-create2-revised primes

thread-create: thread-create.c
	${cc} -pthread -o thread-create thread-create.c

thread-create2: thread-create2.c
	${cc} -pthread -o thread-create2 thread-create2.c

thread-create2-revised: thread-create2-revised.c
	${cc} -pthread -o thread-create2-revised thread-create2-revised.c

primes: primes.c
	${cc} -Wno-int-to-void-pointer-cast -pthread -o primes primes.c

