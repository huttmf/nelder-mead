# for Mac OS
# make all "CFLAGS=-Wall -DMACOSX"

CC= gcc

CFLAGS= -Wall

LIBS= -lm

PROGS = rosenbrock maxpower

all: rosenbrock maxpower

nmsimplex: nmsimplex.c
	${CC} ${CFLAGS} -c nmsimplex.o ${LIBS}

rosenbrock: rosenbrock.o nmsimplex.o
	${CC} ${CFLAGS} -o $@ rosenbrock.c nmsimplex.o ${LIBS}

rosenbrock2: rosenbrock2.o nmsimplex.o
	${CC} ${CFLAGS} -o $@ rosenbrock2.c nmsimplex.o ${LIBS}

maxpower: maxpower.o nmsimplex.o
	${CC} ${CFLAGS} -o $@ maxpower.c nmsimplex.o ${LIBS}

clean:
	rm -f ./*.o ${PROGS}
