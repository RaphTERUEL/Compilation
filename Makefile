CC=gcc
CFLAGS=-Wall
LDFLAGS=
EXEC=automate

all:$(EXEC)

$(EXEC):$(EXEC).o
	$(CC) $(CFLAGS) -o $(EXEC) $(EXEC).o $(LDFLAGS)

$(EXEC).o:$(EXEC).c
	$(CC) $(CFLAGS) -o $(EXEC).o -c $(EXEC).c $(LDFLAGS)

clean:
	rm -rf *.o
	rm -rf $(EXEC)
