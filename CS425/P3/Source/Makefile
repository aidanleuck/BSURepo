CFLAGS = -std=gnu99 -Wall

all: clean rules.d receiver sender
rules.d : $(wildcard *.c) $(wildcard *.h)
	gcc -MM $(wildcard *.c) > rules.d
-include rules.d
receiver: receiver.o
	gcc $(CFLAGS) receiver.o -o receiver3986
sender: sender.o
	gcc $(CFLAGS) sender.o -o sender3986

debug: CFLAGS += -g -O0

debug: clean rules.d sender receiver


debugSender: debug
	gdb -ex 'set args 127.0.0.1 12345 < sender.c' sender3986 
	
debugReceiver: debug
	gdb -ex 'set args 12345' receiver3986

clean:
	rm -f sender.o receiver.o receiver3986 sender3986 rules.d
