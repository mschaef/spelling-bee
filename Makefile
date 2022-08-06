
CFLAGS	= -Wall -g

SRC = spelling-bee.c

all: spelling-bee

spelling-bee: $(SRC)
	gcc $(CFLAGS) -o $@ $(SRC)

clean:
	rm -f *.o *~ spelling-bee
