CFLAGS = -c -Wall
CC = gcc
LIBS =  -lm 

all: client server 

server: server.o protocol.o helper.o handle.o list.o
	${CC} protocol.o helper.o server.o handle.o list.o -o server

server.o: server.c
	${CC} ${CFLAGS} server.c

client: client.o protocol.o helper.o
	${CC} protocol.o helper.o client.o -o client

client.o: client.c 
	${CC} ${CFLAGS} client.c

protocol.o: protocol.c
	${CC} ${CFLAGS} protocol.c

helper.o: helper.c
	${CC} ${CFLAGS} helper.c
	
handle.o: handle.c 
	${CC} ${CFLAGS} handle.c 

list.o: list.c
	${CC} ${CFLAGS} list.c
clean:
	rm -f *.o *~