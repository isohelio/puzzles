CFLAGS = -O4 -DNDEBUG -Wall -march=native -fomit-frame-pointer -Ilibs
# CFLAGS = -pg -g -Wall -DNDEBUG -march=native -Ilibs

CC = gcc

O = iqfit.o libs/cJSON.o libs/pdfgen.o

all: iqfit

iqfit: $O iqfit.h
		gcc -o iqfit $O $(CFLAGS) -lm

clean:
	rm -f *.o *.obj *.exe *.stackdump gmon.out libs/*.o libs/*.obj

solve-iqlove-heart:
	./iqfit -g config/iqlove_heart.json -t 100 -p 1 -O6

solve-iqfit:
	./iqfit -g config/iqfit.json -t 1 -p 1