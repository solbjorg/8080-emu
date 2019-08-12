IDIR=include
SRCDIR=src
BINDIR=bin
CC=gcc
CFLAGS=-I$(IDIR)
OBJ=$(SRCDIR)/main.o $(SRCDIR)/operations.o $(SRCDIR)/resolves.o $(SRCDIR)/types.o $(SRCDIR)/disassembler.o
DEPS=operations.h resolves.h main.h types.h disassembler.h

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

emumake: $(OBJ)
	$(CC) -o bin/8080emu $^ $(CFLAGS) -lSDL2
	rm -f $(SRCDIR)/*.o

clean:
	rm -f $(BINDIR)/* $(SRCDIR)/*.o
