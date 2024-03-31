CFLAGS = -O4 -DNDEBUG -Wall -march=native -fomit-frame-pointer
# CFLAGS = -pg -g -Wall -DNDEBUG -march=native

CC = gcc

O = iqfit.o cJSON.o pdfgen.o

OLOVE = iqlove.o cJSON.o pdfgen.o

$O $(OLOVE): iqfit.h

all: iqfit iqlove

iqfit: $O iqfit.h
		gcc -o iqfit $O $(CFLAGS) -lm

iqlove: $(OLOVE) iqfit.h
		gcc -o iqlove $(OLOVE) $(CFLAGS) -lm

clean:
	rm -f *.o *.obj *.exe *.stackdump gmon.out
