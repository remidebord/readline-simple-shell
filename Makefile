ifndef CC
	CC = gcc
endif

CFLAGS +=-Wall -O
LDLIBS +=-lreadline

PROG=readline-simple-shell

all: $(PROG)

debug: CFLAGS = -Wall -O -g -DDEBUG
debug: ${PROG}

main.o: main.c
	$(CC) $(CFLAGS) -c main.c 

$(PROG): main.o
	$(CC) -o $(PROG) main.o $(LDLIBS)

clean:
	rm -f *.o $(PROG)

