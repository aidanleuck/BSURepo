CFLAGS = -Wall

all: clean rules.d receiver sender
rules.d : $(wildcard *.c) $(wildcard *.h)
	gcc -MM $(wildcard *.c) > rules.d
-include rules.d
receiver: receiver.o
	gcc $(CFLAGS) receiver.o -o receiver
sender: sender.o
	gcc $(CFLAGS) sender.o -o sender

debug: CFLAGS += -g -O0

debug: clean rules.d sender receiver


debugSender: debug
	gdb -ex 'set args 127.0.0.1 12345 < v2.py' sender 
	
debugReceiver: debug
	gdb -ex 'set args 12345' receiver

clean:
	rm -f sender.o receiver.o receiver sender rules.d
