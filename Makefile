CFLAGS = -O4 -DNDEBUG -Wall -march=native -fomit-frame-pointer
# CFLAGS = -pg -g -Wall -DNDEBUG -march=native

CC = gcc

O = iqfit.o cJSON.o pdfgen.o

all: iqfit

iqfit: $O iqfit.h
		gcc -o iqfit $O $(CFLAGS) -lm

clean:
	rm -f *.o *.obj *.exe *.stackdump gmon.out
