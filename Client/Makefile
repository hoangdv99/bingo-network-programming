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

ClientGUI.o: src/ClientGUI.c
	$(CC) -c $(CCFLAGS) src/ClientGUI.c $(GTKLIB) -o ClientGUI.o

clean:
	rm -f *.o $(TARGET)
