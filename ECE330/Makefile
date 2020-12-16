CFLAGS = -std=c99 -Wall

all: main
main: main.o
	gcc $(CFLAGS) main.o -o main
debug: CFLAGS += -g -O0
debug: clean main
	gdb main

clean:
	rm -rf main main.o