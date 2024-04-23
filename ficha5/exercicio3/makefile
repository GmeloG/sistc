CFLAGS = -Wall
LDFLAGS = -pthread


ex3: ex3.o myprint.o 

all: ex3 ficha-sinc-ficheiros.zip

ficha-sinc-ficheiros.zip: myprint.c ex3.c makefile
	-rm $@
	zip $@ $^

clean:
	-rm *.o ex3 *.zip
