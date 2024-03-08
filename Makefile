CFLAGS = -O4 -DNDEBUG -Wall -march=native -fomit-frame-pointer
# CFLAGS = -pg -g -Wall -DNDEBUG -march=native

all: iqfit

iqfit: iqfit.o iqfit.h
		gcc -o iqfit iqfit.o $(CFLAGS) -lm

clean:
	rm -f *.o *.obj *.exe *.stackdump gmon.out
