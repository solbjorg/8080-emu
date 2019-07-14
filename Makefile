IDIR=include
SRCDIR=src
BINDIR=bin
CC=gcc
CFLAGS=-I$(IDIR)
OBJ=$(SRCDIR)/main.o $(SRCDIR)/operations.o $(SRCDIR)/resolves.o
DEPS=operations.h resolves.h main.h

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

emumake: $(OBJ)
	$(CC) -o bin/8080emu $^ $(CFLAGS)
	rm -f $(SRCDIR)/*.o

clean:
	rm -f $(BINDIR)/* $(SRCDIR)/*.o
