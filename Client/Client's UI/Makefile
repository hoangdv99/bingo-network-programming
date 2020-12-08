# change application name here (executable output name)
TARGET=Client

# compiler
CC=gcc
# debug
DEBUG=-g
# optimisation
OPT=-O0
# warnings
WARN=-Wall

PTHREAD=-pthread

CCFLAGS=$(DEBUG) $(OPT) $(WARN) $(PTHREAD) -pipe

GTKLIB=`pkg-config --cflags --libs gtk+-3.0`

# linker
LD=gcc
LDFLAGS=$(PTHREAD) $(GTKLIB) -export-dynamic

OBJS=    Main.o CheckString.o

all: $(OBJS)
	$(LD) -o $(TARGET) $(OBJS) $(LDFLAGS)

Main.o: src/Main.c
	$(CC) -c $(CCFLAGS) src/Main.c $(GTKLIB) -o Main.o

CheckString.o: src/CheckString.c
	$(CC) -c $(CCFLAGS) src/CheckString.c -o CheckString.o

clean:
	rm -f *.o $(TARGET)
