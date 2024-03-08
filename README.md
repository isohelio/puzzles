## Yet Another IQ FIT Puzzle Solver

There are many like it, but this one is mine.

### Introduction

A straight forward, recursive search algorithm for finding all solutions of the IQ FIT puzzle.

### Installation

The software should run on any linux type system with gcc.

1. Clone/download the repo.
2. In the repo folder
    
    make clean all

3. Run the software with no arguments to generate all the solutions to the puzzle that contain no unfilled spaces. This takes about 2 minutes.

    ./iqfit

4. To capture the solutions into a file

    ./iqfit -o solutions.txt

5. To see command line options

    ./iqfit -h

### Results

Total solutions without gaps


    iqfit -p 1000

    total solutions 301350, invalid solutions 57472

Total solutions with gaps

    iqfit -O1 -p 1000

    total solutions 6462189, invalid solutions 0


