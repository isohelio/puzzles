#ifndef IQFIT_H
#define IQFIT_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>
#include <malloc.h>
#include <math.h>
#include <getopt.h>
//#include <zlib.h>
//#include <time.h>
//#include <locale.h>

#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))
#define LEN(x)	(sizeof(x)/sizeof(x[0]))

typedef unsigned long long piece_hash;

typedef struct orientation {
	int position[16][2];
	int position_count;
	int width;
	int height;
} orientation;

typedef struct piece {
	char *name;						// piece name, the colour
	char *colour;					// the linux terminal colour escape code
	char *abbreviation;				// single letter abbreviation for the piece
	char *layout[2][2];				// the different layouts for each piece
	orientation orientation[16];	// the piece orientations for all layouts
	int orientation_count;			// number of orientations
	int id;							// integer id that is used for tracking board positions
	int inactive;					// piece is not active in the search
} piece;

piece pieces[] = {
		{ "red", 		"31", "R", {	{"****",
								  	  	 "*  *"},
										{"   *",
										 "****"},}
		},
		{ "orange",		"33", "O", {	{" * *",
										 "****"},
										{"  * ",
										 "****"}, }
		},
		{ "yellow", 	"93", "Y", {	{"*   ",
										 "****"},
										{"**  ",
										 "****"}, }
		},
		{ "cyan",		"96", "C", {	{"  * ",
										 "****"},
										{" ** ",
										 "****"}, }
		},
		{ "lightblue",	"94", "b", {	{"* * ",
										 "****"},
										{"   *",
										 "****"}, }
		},
		{ "pink",		"95", "p", {	{" *  ",
										 "****"},
										{"  **",
										 "****"},}
		},
		{ "lightgreen",	"92", "g", {	{"* *",
										 "***"},
										{"  *",
										 "***"},}
		},
		{ "blue",		"34", "B", {	{"* *",
										 "***"},
										{" * ",
										 "***"},}
		},
		{ "green",		"32", "G", {	{" * ",
										 "***"},
										{" **",
										 "***"},}
		},
		{ "purple",		"35", "P", {	{"** ",
										 "***"},
										{"*  ",
										 "***"}, }
 		},
};

#define BOARD_WIDTH		10
#define BOARD_HEIGHT	5

typedef struct board {
	int *piece_id;
	int *region_id;
	int region_pieces[BOARD_WIDTH][BOARD_HEIGHT];
	int region_size[BOARD_WIDTH * BOARD_HEIGHT];
	int region_count;
	unsigned long long search_calls[128];
	int width;
	int height;
	piece *pieces;
	int solutions;
	int invalid_solutions;
	int print_frequency;
	int terminate;
	int debug;
	int verbose;
	int advanced;
	int optimisations;
	int total_placements;
	int solution_placements;
	int no_symmetry;
	char *output_filename;
	char *input_filename;
	char *solve_filename;
	FILE *output_fp;
} board;

#endif /* IQFIT_H */
