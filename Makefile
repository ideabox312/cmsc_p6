TARGETS = test 
CFLAGS = -c -Wall 
CC = gcc

all: $(TARGETS)	

test.o: test.c bitmap.h


bitmap.o: bitmap.c bitmap.h

bencode.o: bencode.c bencode.h

utils.o: utils.c utils.h

sha1.o: sha1.c sha1.h

geek_hdo.o: geek_hdo.c geek_hdo.h

test: test.o bitmap.o bencode.o utils.o sha1.o geek_hdo.o
	$(CC) -o test test.o bitmap.o 

clean:
	rm -f *.o *.txt $(TARGETS)

