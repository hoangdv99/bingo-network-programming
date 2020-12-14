CC = gcc
DEBUG = -g
OPT = -O0
WARN = -Wall
PTHREAD = -pthread
GTKLIB = `pkg-config --cflags --libs gtk+-3.0`
CFLAGS = $(DEBUG) $(OPT) $(WARN) $(PTHREAD) -pipe -fPIE

all: client server 

OBJ_CLIENT = client.o protocol.o helper.o client_handle.o Client/ClientGUI.o
OBJ_SERVER = server.o protocol.o helper.o handle.o list.o

%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS)

server: $(OBJ_SERVER)
	${CC} -o $@ $^ $(CFLAGS)

client: $(OBJ_CLIENT)
	${CC} -o $@ $^ $(CFLAGS) ${GTKLIB} -export-dynamic

clean:
	rm -f *.o client server