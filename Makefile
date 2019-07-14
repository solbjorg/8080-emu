IDIR=include
SRCDIR=src
BINDIR=bin
CC=gcc
CFLAGS=-I$(IDIR)
OBJ=$(SRCDIR)/main.o $(SRCDIR)/operations.o
DEPS=operations.h

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

emumake: $(OBJ)
	$(CC) -o bin/8080emu $^ $(CFLAGS)

clean:
	rm -f $(BINDIR)/* $(SRCDIR)/*.o
