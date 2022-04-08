webserver: webserver.o threading.o strings.o
	gcc -pthread webserver.o threading.o strings.o -o webserver

webserver.o: webserver.c user.h
	gcc -c -w -pthread webserver.c

threading.o: threading.c threading.h
	gcc -c -pthread threading.c

strings.o: strings.c strings.h
	gcc -c -pthread strings.c

clean:
	rm *.o webserver
