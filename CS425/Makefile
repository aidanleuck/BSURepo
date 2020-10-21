CFLAGS = -Wall

all: rules.d receiver sender
rules.d : $(wildcard *.c) $(wildcard *.h)
	gcc -MM $(wildcard *.c) > rules.d
-include rules.d
receiver: receiver.o
	gcc $(CFLAGS) receiver.o -o receiver
sender: sender.o
	gcc $(CFLAGS) sender.o -o sender

clean:
	rm -f sender.o receiver.o receiver sender rules.d
