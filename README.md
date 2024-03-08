## Yet Another IQ FIT Puzzle Solver

There are many like it, but this one is mine.

### Introduction

A straight forward, recursive search algorithm for finding all solutions of the IQ FIT puzzle.

### Installation

The software should run on any linux type system with gcc.

1. Clone/download the repo.
2. Run make in the repo folder, which should produce output like this
    
        make clean all
        rm -f *.o *.obj *.exe *.stackdump gmon.out
        cc -O4 -DNDEBUG -Wall -march=native -fomit-frame-pointer   -c -o iqfit.o iqfit.c
        gcc -o iqfit iqfit.o -O4 -DNDEBUG -Wall -march=native -fomit-frame-pointer -lm

3. Run the software with no arguments to generate all the solutions to the puzzle that contain no unfilled spaces. This takes about 2 minutes.

        ./iqfit

4. To capture the solutions into a file

        ./iqfit -o solutions.txt

5. To see command line options

        ./iqfit -h

### Usage

    usage: iqfit [options]

    solve problems from the iqfit puzzle
    -p n       print every n'th solution                              (default: 1000)
    -t n       terminate after the n'th solution                      (default: none)
    -d         show every step of solving the board                   (default: off)
    -On        optimisation level 0-4                                 (default: 10!)
    -o file    print all solutions to file                            (default: none)
    -i file    read solutions from the specified file                 (default: none)
    -S file    solve examples from the specified file                 (default: none)
    -D abbrevs deactivate the pieces specified by their abbreviations (default: none)
    -s         only generate the non-symmetry related solutions
    -h         print this message

    default behaviour is to enumerate all solutions, using all optimisations

    -O0        disable all optimisations, and allow solutions that include gaps
    -O1        generate all solutions that don't include any gaps
    -O2        skip partial solutions that include 1, 2 or 3 size gaps
    -O3        skip partial solutions that include some impossible shapes
    -O4        skip partial solutions that include size 7 gaps



### Results

Total solutions without gaps


    iqfit -p 1000

    total solutions 301350, invalid solutions 57472

Total solutions with gaps

    iqfit -O0 -p 1000

    total solutions 6462189, invalid solutions 0


