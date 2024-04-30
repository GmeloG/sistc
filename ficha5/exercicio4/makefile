CFLAGS = -Wall
LDFLAGS = -lrt -pthread -lm

default: ex4-reader ex4-writer

ex4-reader.o: ex4-reader.c 
			gcc $(CFLAGS) -c ex4-reader.c 
ex4-writer.o: ex4-writer.c 
			gcc $(CFLAGS) -c ex4-writer.c

ex4-reader: ex4-reader.o
			gcc $(CFLAGS) ex4-reader.o -o ex4-reader $(LDFLAGS)
ex4-writer: ex4-writer.o
			gcc $(CFLAGS) ex4-writer.o -o ex4-writer $(LDFLAGS)

clean:
	-rm *.o
