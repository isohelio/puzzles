## Yet Another IQ FIT Puzzle Solver

There are many like it, but this one is mine.

### Introduction

A straight forward, recursive search algorithm for finding all solutions of the IQ FIT puzzle.

### Installation

The software should run on any linux type system with gcc.

1. Clone/download the repo.
2. Run make in the repo folder, which should produce output like this
    
        > make clean all
        rm -f *.o *.obj *.exe *.stackdump gmon.out
        cc -O4 -DNDEBUG -Wall -march=native -fomit-frame-pointer   -c -o iqfit.o iqfit.c
        gcc -o iqfit iqfit.o -O4 -DNDEBUG -Wall -march=native -fomit-frame-pointer -lm

3. Run the software with no arguments to generate all the solutions to the puzzle that contain no unfilled spaces. This takes about 2 minutes.

        > ./iqfit

4. To capture the solutions into a file

        > ./iqfit -o solutions.txt

5. To see command line options

        > ./iqfit -h

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

### Piece definitions

The IQ FIT pieces are currently hard wired into the software (see `iqfit.h`). Most of the colours are self explanatory 
but the blue colour names have been taken to conincide with the physical pieces rather than the instruction manual.

        1 red          ****     *
          R 31         *  *  ****

        2 orange        * *    *
          O 33         ****  ****

        3 yellow       *     **
          Y 93         ****  ****

        4 cyan           *    **
          C 96         ****  ****

        5 lightblue    * *      *
          b 94         ****  ****

        6 pink          *      **
          p 95         ****  ****

        7 lightgreen   * *    *
          g 92         ***  ***

        8 blue         * *   *
          B 34         ***  ***

        9 green         *    **
          G 32         ***  ***

        10 purple       **   *
          P 35         ***  ***

### Solve Puzzles

Partial solutions can be provided using a simple text file format, which consists of 

* a title
* 5 lines of 10 characters using the piece abbreviations
* unfilled positions can be indicated with a - or _ character

There are examples in the examples folder e.g.

        STARTER-1
        ggg-CCCCGG
        gBg-RCCGGG
        BB--RPYYYY
        bBb-RPPP-Y
        bbbbRR----

To solve the puzzle run

        ./iqfit -S examples/examples.txt -p 1

Output will look like this

![image](https://github.com/isohelio/puzzles/assets/38924166/404b84f9-71d0-4883-a004-3b5d0908dee8)


Speciy `-o filename` to capture the puzzle solutions to a text file.

### Results

Total solutions without gaps


    iqfit -p 1000

    total solutions 301350, invalid solutions 57472

Total solutions with gaps

    iqfit -O0 -p 1000

    total solutions 6462189, invalid solutions 0


