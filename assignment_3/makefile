all: client.out server.out

server.out: server.o socket.o
	gcc socket.o server.o -o server.out

client.out: client.o socket.o
	gcc client.o socket.o -o client.out

socket.o: socket.h socket.c
	gcc -c socket.c

server.o: server.c
	gcc -c server.c

client.o: client.c
	gcc -c client.c

clean:
	rm *.out
	rm *.o