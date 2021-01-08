# CC = gcc
# DEBUG = -g
# OPT = -O0
# WARN = -Wall
# PTHREAD = -pthread
# GTKLIB = `pkg-config --cflags --libs gtk+-3.0`
# CFLAGS = $(DEBUG) $(OPT) $(WARN) $(PTHREAD) -pipe -fPIE

# all: client server 

# OBJ_CLIENT = client.o protocol.o helper.o client_handle.o ClientGUI.o
# OBJ_SERVER = server.o protocol.o helper.o handle.o list.o gameplay.o

# %.o: %.c
# 	$(CC) -c -o $@ $< $(CFLAGS)

# ClientGUI.o: Client/src/ClientGUI.c
# 	$(CC) -c $(CCFLAGS) Client/src/ClientGUI.c $(GTKLIB) -o ClientGUI.o

# server: $(OBJ_SERVER)
# 	${CC} -o $@ $^ $(CFLAGS)

# client: $(OBJ_CLIENT)
# 	${CC} -o $@ $^ $(CFLAGS) ${GTKLIB} -export-dynamic

# clean:
# 	rm -f *.o client server main test *~


## hoang test
CFLAGS = -c -Wall
CC = gcc
LIBS =  -lm 

all: client server 

server: server.o protocol.o helper.o handle.o list.o gameplay.o
	${CC} protocol.o helper.o server.o handle.o list.o gameplay.o -o server -lpthread

server.o: server.c
	${CC} ${CFLAGS} server.c -lpthread

client: client.o protocol.o helper.o client_handle.o 
	${CC} protocol.o helper.o client_handle.o client.o -o client -lpthread

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

gameplay.o: gameplay.c
	${CC} ${CFLAGS} gameplay.c -lpthread

clean:
	rm -f *.o *~