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

#include "cJSON.h"
#include "pdfgen.h"

#define BOARD_MAX_PIECES		10
#define BOARD_MAX_ORIENTATIONS	32
#define BOARD_MAX_POSITIONS		32
#define BOARD_MAX_OUTLINES		32
#define BOARD_MAX_LAYOUTS		32

#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))
#define LEN(x)	(sizeof(x)/sizeof(x[0]))

typedef unsigned long long board_bitmap;

typedef unsigned long long piece_hash;

typedef struct orientation {
	int position[BOARD_MAX_POSITIONS][2];
	int position_count;
	int width;
	int height;
	piece_hash hash;
} orientation;

typedef struct piece_layout {
	char lines[256][10];
	int line_count;
} piece_layout;

typedef struct piece {
	char *name;											// piece name, the colour
	char *colour;										// the linux terminal colour escape code
	char *abbreviation;									// single letter abbreviation for the piece
	piece_layout layout[BOARD_MAX_LAYOUTS];
	int layout_count;
	// char *layout[2][2];									// the different layouts for each piece
	orientation orientation[BOARD_MAX_ORIENTATIONS];	// the piece orientations for all layouts
	int orientation_count;								// number of orientations
	int id;												// integer id that is used for tracking board positions
	int inactive;										// piece is not active in the search
	float outline[BOARD_MAX_OUTLINES];					// outline of the pieces for iqlove
	int outline_count;									// number of points in the outline (*2)
	unsigned int rgb_fill;								// fill colour of piece in ARGB
	unsigned int rgb_outline;							// outline colour of piece in ARGB
	int symmetric;										// does the piece have rotational symmetry
} piece;

#define BOARD_WIDTH		10
#define BOARD_HEIGHT	5

typedef struct board {
	piece pieces[BOARD_MAX_PIECES];
	int piece_count;
	int flip_pieces;
	int *piece_id;
	int *region_id;
	int region_pieces[BOARD_WIDTH][BOARD_HEIGHT];
	int region_size[BOARD_WIDTH * BOARD_HEIGHT];
	int region_points[4 * BOARD_WIDTH * BOARD_HEIGHT];
	int region_count;
	unsigned long long search_calls[128];
	int width;
	int height;
	int step;
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
	char *method;
	char *config_filename;
	char *output_filename;
	char *input_filename;
	char *solve_filename;
	FILE *output_fp;
} board;

#endif /* IQFIT_H */
