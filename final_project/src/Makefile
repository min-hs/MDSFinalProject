all:
	gcc -c main.c
	gcc -c socket_server.c
	gcc -o test main.o socket_server.o -lpthread
clean:
	rm -rf *.o

t:
	sudo ./test 3501
