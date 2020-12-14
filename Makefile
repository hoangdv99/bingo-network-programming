# CC = gcc
# DEBUG = -g
# OPT = -O0
# WARN = -Wall
# PTHREAD = -pthread
# GTKLIB = `pkg-config --cflags --libs gtk+-3.0`
# CFLAGS = $(DEBUG) $(OPT) $(WARN) $(PTHREAD) -pipe -fPIE

# all: client server 

# OBJ_CLIENT = client.o protocol.o helper.o client_handle.o Client/ClientGUI.o
# OBJ_SERVER = server.o protocol.o helper.o handle.o list.o

# %.o: %.c
# 	$(CC) -c -o $@ $< $(CFLAGS)

# server: $(OBJ_SERVER)
# 	${CC} -o $@ $^ $(CFLAGS)

# client: $(OBJ_CLIENT)
# 	${CC} -o $@ $^ $(CFLAGS) ${GTKLIB} -export-dynamic

# clean:
# 	rm -f *.o *~
CFLAGS = -c -Wall
CC = gcc
LIBS =  -lm 

all: client server 

server: server.o protocol.o helper.o handle.o list.o
	${CC} protocol.o helper.o server.o handle.o list.o -o server

server.o: server.c
	${CC} ${CFLAGS} server.c

client: client.o protocol.o helper.o client_handle.o
	${CC} protocol.o helper.o client_handle.o client.o -o client

client.o: client.c 
	${CC} ${CFLAGS} client.c

protocol.o: protocol.c
	${CC} ${CFLAGS} protocol.c

helper.o: helper.c
	${CC} ${CFLAGS} helper.c
	
handle.o: handle.c 
	${CC} ${CFLAGS} handle.c 

client_handle.o: client_handle.c
	${CC} ${CFLAGS} client_handle.c
list.o: list.c
	${CC} ${CFLAGS} list.c
clean:
	rm -f *.o *~