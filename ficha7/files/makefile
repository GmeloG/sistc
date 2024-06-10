CFLAGS=-Wall

SANITIZE_CF=-fsanitize=address -fsanitize=undefined -fno-sanitize-recover=all -fsanitize=float-divide-by-zero -fsanitize=float-cast-overflow -fno-sanitize=null -fno-sanitize=alignment
SANITIZE_LDF=$(SANITIZE_CF) -lasan
#CFLAGS=-Wall -std=c17 -D _XOPEN_SOURCE=700 $(SANITIZE_CF)
#LDFLAGS=$(SANITIZE_LDF)


all: veic_clt ex1_template

veic_clt: veic_clt.o json.o
	gcc -o $@  $^ $(LDFLAGS)

veic_clt.o: json.h

clean:
	rm *.o veic_clt ex1_template
