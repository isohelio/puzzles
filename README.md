## Yet Another IQ FIT Puzzle Solver

There are many like it, but this one is mine.

### Introduction

A recursive search algorithm for finding all solutions of the IQ FIT puzzle.

![image](https://github.com/isohelio/puzzles/assets/38924166/a4f86dbb-4a9a-4ddb-b074-2a31ba0e3487)

### Installation

The software should run on any linux type system with `gcc` and `Python` for some post processing.

1. Clone/download the repo.

        git clone https://github.com/isohelio/puzzles

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
        -g file    configuration for the game                             (default: configurations/iqfit.json)
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

### Game Configuration

The IQFIT pieces and board are now specified in a `.json` configuration file. The configuration can be specified with the -g option. The default is for the original IQFIT game and is contained in [`configurations/iqfit.json`](https://github.com/isohelio/puzzles/blob/main/configurations/iqfit.json). You can view the piece definitions in the configuration file.

Most of the colours are self explanatory 
but the blue colour names have been taken to coincide with the physical pieces rather than the instruction manual. The colours are specified in linux terminal escape code colour numbers.


```
{
    "width":    10,
    "height":   5,
    "step":     1,
    "pieces": [
        {
            "name": "red", 		"colour": "31", "abbreviation": "R", "rgb": "230,0,28",
            "layout": [ [   "****",
                            "*  *"],
                        [   "   *",
                            "****"] ]
        },
        {
            "name": "orange",		"colour": "33", "abbreviation": "O", "rgb": "255,193,80",
            "layout": [ [   " * *",
                            "****"],
                        [   "  * ",
                            "****"] ]
        },
        {
            "name": "yellow", 	"colour": "93", "abbreviation": "Y", "rgb": "255,255,160",
            "layout": [ [   "*   ",
                            "****"],
                        [    "**  ",
                            "****"] ]
        },
        {
            "name": "cyan",		"colour": "96", "abbreviation": "C", "rgb": "0,201,255",
            "layout": [ [   "  * ",
                            "****"],
                        [   " ** ",
                            "****"] ]
        },
        {
            "name": "lightblue",	"colour": "94", "abbreviation": "b", "rgb": "0,182,255",
            "layout":   [ [ "* * ",
                            "****"],
                        [   "   *",
                            "****"] ]
        },
        {
            "name": "pink",		"colour": "95", "abbreviation": "p", "rgb": "243,116,187",
            "layout":   [ [ " *  ",
                            "****"],
                        [   "  **",
                            "****"] ]
        },
        {
            "name": "lightgreen",	"colour": "92", "abbreviation": "g", "rgb": "194,205,90",
            "layout":   [ [ "* *",
                            "***"],
                        [   "  *",
                            "***"] ]
        },
        {
            "name": "blue",		"colour": "34", "abbreviation": "B", "rgb": "0,84,202",
            "layout":   [ [ "* *",
                            "***"],
                        [   " * ",
                            "***"] ]
        },
        {
            "name": "green",		"colour": "32", "abbreviation": "G", "rgb": "0,100,100",
            "layout":   [ [ " * ",
                            "***"],
                        [   " **",
                            "***"] ]
        },
        {
            "name": "purple",		"colour": "35", "abbreviation": "P", "rgb": "47,0,156",
            "layout":   [ [ "** ",
                            "***"],
                        [   "*  ",
                            "***"] ]
        }
    ]
}
```

### Solve Puzzles

Partial solutions can be provided using a simple text file format, which consists of 

* A title
* 5 lines of 10 characters using the piece abbreviations
* Unfilled positions can be indicated with a - or _ character

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


Specify `-o filename` to capture the puzzle solutions to a text file.

Alternatively if the title line is longer than 50 characters it is interpreted as a single line solution format e.g.

        RRRRgggObbRPYRgOOOObPPYYYYBBBbPPppGGGBCbppppGGCCCC


### Results

Total [solutions](https://github.com/isohelio/puzzles/blob/main/solutions/solutions.txt) without gaps


    iqfit -p 1000

    total solutions 301350, invalid solutions 57472

Total solutions with gaps

    iqfit -O0 -p 1000

    total solutions 6462189, invalid solutions 0

### Solving Puzzles Without Using A Specified Piece

There are a maximum of 56 nodes available from the 10 pieces. Therefore the 50 holes in the puzzle can be solved by omitting one piece.

Puzzle pieces can be deactivated using the `-D` flag e.g. to solve the puzzle without the cyan (C) piece

    ./iqfit -DC -p1

This produces the following solutions.

![image](https://github.com/isohelio/puzzles/assets/38924166/a3a3fd02-7a44-4d74-9e4f-b8918f57d828)


### Output Format

The solutions can be captured in an output file, specified with the `-o` option.

The solutions are stored one per line, printed from the top left corner. The piece positions are indicated using the piece abbreviation. A few lines from the file are shown below

        RRRRgggObbRPYRgOOOObPPYYYYBBBbPPppGGGBCbppppGGCCCC
        RRRRgggObbRGGRgOOOObGGGppCCCCbYppppBCPPbYYYYBBBPPP
        RRRRgggOBBRCCRgOOOOBCCCCbGGGBBYbbbbpGPPPYYYYppppPP
        RRRRgggOBBRGGRgOOOOBGGGCCCCPBBppppCbYPPPppbbbbYYYY
        RRRRgggObbRYYRgOOOObGYYYYppppbGGPPBBBpCbGGPPPBCCCC
        RRRRgggObbRYYRgOOOObGYYYYppppbGGCCPPPpBbGCCCCPPBBB
        RRRRgggObbRYYRgOOOObBYYYYppppbBBCCPPPpGbBCCCCPPGGG
        RRRRCCCCGGRPPROCCBGGgPOOOObBBGgPYbbbbBppggYYYYpppp

The single solution per line format is convenient for working with the solutions in command line tools.

### Python Helper Program

Most solution analyses are done with Python as it is mainly just reading the single line solution output files.

The image generation parts of this require the python `png` module which is installed with 

        pip install pypng

### Generate PNG Image of Solutions

With the `--image` option you can generate a png of all the solutions in a file. The `--columns` option specifies how many solutions to put in one row.

The images are a bit hard to handle but are quite neat to look at. The images have one pixel per section of the piece. Unfilled pieces are shown with a light grey checker board pattern.

*Note that the default Photo Viewer in Windows 11 will not display these images correctly. It insists on interpolating the colours as you zoom in. ImageGlass will display them correctly.*

    python iqfit.py --image --input solutions/solutions.txt \
              --columns 500 --output solutions/solutions.png

Here is a section from the full solution set image.

![image](https://github.com/isohelio/puzzles/assets/38924166/49262361-a43c-4c71-bf52-0569e980f08b)

### Generate Two and Three Piece Start Points With Unique Solutions

The Python helper program can generate the start points with two or three pieces that have a unique solution

    python iqfit.py --two --input solutions/solutions.txt \
                          --output solutions/two_piece_solutions.txt --dedup

The `--dedup` option deduplicates the output by removing one pair of each of the solutions that are related by 180&deg; rotation of the board. There are 13789 puzzles with [two pieces](https://github.com/isohelio/puzzles/blob/main/solutions/two_piece_solutions.txt) that generate a single solution. A small sample is shown below.

![image](https://github.com/isohelio/puzzles/assets/38924166/50c83377-c887-4436-be5d-d74e63dcc78d)

Use the `--three` option to generate all three piece puzzles that have a unique solution. There are 1209312 [three piece](https://github.com/isohelio/puzzles/blob/main/solutions/three_piece_solutions.txt) solutions.

![image](https://github.com/isohelio/puzzles/assets/38924166/584ae245-af6e-4a36-8ef8-e909f27166e0)

#### Algorithm

The unique solutions are generated by taking each solution in turn and replacing all piece names with _ apart from the two or three pieces that are being considered. The partial solutions are then sorted together and the ones that only occur once are output as a solution.

    ________B__CC____BB_CCCC____B_____________________ 1
    ___________CC____BBBCCCC____B_____________________ 1
    ___________CC_BBB___CCCCB_B_______________________ 1
    _______C_________C_____B_B_CC____BBB_C____________ 1
    ____C_________CC_______BCC______BBC________B______ 1

The solutions are deduplicated by generating the rotated version of the solution, and only writing it if the initial version is alphabetically less then the rotated version. This works for both complete solutions and partial solutions.
