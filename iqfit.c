#include "iqfit.h"

#define ForEachPiece(p) for(piece *p = pieces; p < pieces + LEN(pieces); p++)
#define ForEachOrientation(o, p) for(orientation *o = p->orientation, *end = p->orientation + p->orientation_count; o < end; o++)
#define ForEachPosition(p, ori) for(int p = 0, end = ori->position_count; p < end; p++)

piece_hash
generate_piece_hash(int *xy, int count){
	piece_hash hash = 0;
	int xmin = INT32_MAX, ymin = INT32_MAX;

	for(int i = 0; i < count; i += 2){
		xmin = MIN(xmin, xy[i]);
		ymin = MIN(ymin, xy[i + 1]);
	}

	for(int i = 0; i < count; i += 2){
		// printf("%d %d\n", xy[i], xy[i + 1]);
		int bit = xy[i] - xmin;
		int word = xy[i + 1] - ymin;
		hash |= (piece_hash)((piece_hash)1 << ((8 * word) + bit));
	}

	// printf("hash %lld\n", hash);

	return hash;
}

int
orientation_store_position(orientation *ori, int x, int y){
	ori->position[ori->position_count][0] = x;
	ori->position[ori->position_count][1] = y;

	return ori->position_count++;
}

void
orientation_print(orientation *ori){
	ForEachPosition(p, ori){
		printf("%d:", p);
		printf(" [%3d, %3d], ", ori->position[p][0], ori->position[p][1]);
	}
	printf("\n");
}

void
initialise_piece(board *b, piece *p){
	if(b->verbose) printf("initialising %s\n", p->name);

	p->orientation_count = 0;

	for(int l = 0; l < LEN(p->layout); l++){
		for(int rot = 0; rot < 4; rot++){
			orientation *ori = p->orientation + p->orientation_count++;
			ori->position_count = 0;
			int height = 2;
			for(int r = 0; r < LEN(p->layout[l]); r++){
				char *s = p->layout[l][r];
				int width = strlen(s);

				ori->width = width;
				ori->height = height;
				if(rot == 1 || rot == 3){
					ori->width = height;
					ori->height = width;
				}

				// generate the 4 rotations of the piece
				for(int c = 0; c < width; c++){
					if(s[c] != ' '){
						int x = c, y = r;
						if(rot == 1){
							x = height - r - 1, y = c;
						}else if(rot == 2){
							x = width - c - 1, y = height - r - 1;
						}else if(rot == 3){
							x = r, y = width - c - 1;
						}
						orientation_store_position(ori, x, y);
					}
				}
			}

			ori->hash = generate_piece_hash((int *)&ori->position, ori->position_count * 2);
		}
	}

	if(b->verbose){
		printf("orientations %d\n", p->orientation_count);

		ForEachOrientation(ori, p){
			orientation_print(ori);
		}

		printf("count %d\n", p->orientation_count);
	}
}

void
initialise_pieces(board *b){
	int i = 0;
	ForEachPiece(p){
		initialise_piece(b, p);
		p->id = ++i;
	}
}

FILE *
board_fopen(char *filename, char *mode){
	FILE *fp = fopen(filename, mode);
	if(fp == NULL){
		fprintf(stderr, "couldn't open input file %s\n", filename);
		exit(2);
	}

	return fp;
}

#define board_index(b,x,y)				((y)*b->width + (x))
#define board_get_piece(b,x,y) 			b->piece_id[board_index(b,x,y)]
#define board_set_piece(b,x,y,piece)	b->piece_id[board_index(b,x,y)] = piece
#define board_get_region(b,x,y)			b->region_id[board_index(b,x,y)]
#define board_set_region(b,x,y,region)	b->region_id[board_index(b,x,y)] = region


void
board_reset(board *b){
	b->solutions = 0;
	b->invalid_solutions = 0;
	for(int y = 0; y < b->height; y++){
		for(int x = 0; x < b->width; x++){
			board_set_piece(b, x, y, 0);
			board_set_region(b, x, y, 0);
		}
	}

	ForEachPiece(p) p->inactive = 0;
}

char *
board_format_cell(board *b, int x, int y, char *buf){
	int c = board_get_piece(b, x, y);
	// the piece id is one more than the position in the array of pieces
	piece *p = &b->pieces[c-1];

	if(c) sprintf(buf, "\033[%sm%s\033[0m", p->colour, p->abbreviation);
	else sprintf(buf, "_");

	return buf;
}

piece *
board_get_piece_from_abbreviation(board *b, char abbrev){
	ForEachPiece(p){
		if(p->abbreviation[0] == abbrev){
			return p;
		}
	}

	return NULL;
}

void
board_print(board *b){
	char buf[128];
	for(int y = 0; y < b->height; y++){
		for(int x = 0; x < b->width; x++){
			printf(" %s", board_format_cell(b, x, y, buf));
		}
		printf("\n");
	}
}

/**
 * Is the board solved?
 * There can be no empty spaces in a valid solution.
 * */
int
board_solved(board *b){
	for(int y = 0, h = b->height; y < h; y++){
		for(int x = 0, w = b->width; x < w; x++){
			if(board_get_piece(b, x, y) == 0) return 0;
		}
	}

	return 1;
}

int
board_position_occupied(board *b, int ix, int iy){
	if(ix < 0 || ix >= b->width || iy < 0 || iy >= b->height){
		return 1;
	}else if(board_get_piece(b, ix, iy) != 0){
		return 1;
	}
	return 0;
}

void
board_region_propagate(board *b, int x, int y, int region){
	if(board_get_piece(b, x, y) == 0 && board_get_region(b, x, y) == 0){
		board_set_region(b, x, y, region);
		// b->region_points[2*b->region_size[region]] = x;
		// b->region_points[2*b->region_size[region] + 1] = y;
		b->region_size[region]++;
		if(y - 1 >= 0) 			board_region_propagate(b, x,     y - 1, region);
		if(x - 1 >= 0) 			board_region_propagate(b, x - 1, y,     region);
		if(x + 1 < b->width) 	board_region_propagate(b, x + 1, y,     region);
		if(y + 1 < b->height) 	board_region_propagate(b, x,     y + 1, region);
	}
}

int
board_match(board *b, int x, int y, int *empty, int *occupied, int *alternate, int swap){
	if(swap == 0){
		for(int i = 0; empty[i] != 999; i += 2){
			if(board_position_occupied(b, x + empty[i], y + empty[i + 1])) return 0;
		}

		for(int i = 0; occupied[i] != 999; i += 2){
			if(board_position_occupied(b, x + occupied[i], y + occupied[i + 1]) == 0) return 0;
		}

		if(alternate){
			for(int i = 0; alternate[i] != 999; i += 2){
				if(board_position_occupied(b, x + alternate[i], y + alternate[i + 1])) return 1;
			}
			return 0;
		}

	}else{
		for(int i = 0; empty[i] != 999; i += 2){
			if(board_position_occupied(b, x + empty[i + 1], y + empty[i])) return 0;
		}

		for(int i = 0; occupied[i] != 999; i += 2){
			if(board_position_occupied(b, x + occupied[i + 1], y + occupied[i]) == 0) return 0;
		}

		if(alternate){
			for(int i = 0; alternate[i] != 999; i += 2){
				if(board_position_occupied(b, x + alternate[i + 1], y + alternate[i])) return 1;
			}
			return 0;
		}
	}

	return 1;
}

int linear4_empty[] 			= {  0, 1,          0, 2,          0, 3, 999};
int linear4_occupied[] 			= { -1, 0, 1, 0,   -1, 1, 1, 1,   -1, 2, 1, 2,   -1, 3, 1, 3,    0, -1, 0, +4,	999};

int linear4_open_empty[] 		= {  0, 1,          0, 2,          0, 3, 999};
int linear4_open_occupied[] 	= { -1, 0, 1, 0,   -1, 1, 1, 1,   -1, 2, 1, 2,   -1, 3, 1, 3,					999};
int linear4_open_alternate[] 	= {  0, -1,          0, 4,          999};

int dogleg4_up_empty[] 			= {  1, 0,          1, -1,          2, -1, 					999};
int dogleg4_up_occupied[] 		= {  0, -1,   		0, 1, 1, 1,     2, 0,   1, -2, 2, -2, 	999};
int dogleg4_up_alternate[] 		= { -1, 0,          3, -1,           						999};

int dogleg4_down_empty[] 		= {  1, 0,          1,  1,          2, 1, 					999};
int dogleg4_down_occupied[] 	= { -0, -1,			0, 1, 1, 2,     2, 0, 1, 2, 1, 0,		999};
int dogleg4_down_alternate[] 	= { -1, 0,          3,  1,          	 					999};

int square4_empty[] 			= {  1, 0,          0, 1,          1, 1, 999};
int square4_occupied[] 			= {  0, -1, 1, -1,  0,  2, 1, 2,  -1, 0, -1, 1,    2, 0, 2, 1,  999};

int snake_up_empty[] 			= {  0, -1,          1, -1,          1, -2, 		2, -2,		999};
int snake_up_occupied[] 		= {  -1, 0, -1, -1, 0, -2,  1, -3, 2, -3,   3, -2, 2, -1, 1, 0, 0, 1, 999};

int
board_match_shape(board *b, int x, int y, int *empty, int *occupied, int *alternate){
	if(board_match(b, x, y, empty, occupied, alternate, 0)) return 1;
	if(board_match(b, x, y, empty, occupied, alternate, 1)) return 1;
	return 0;
}

int
board_solvable_custom(board *b){
	for(int y = 0; y < b->height; y++){
		for(int x = 0, w = b->width; x < w; x++){
			if(board_get_piece(b, x, y) == 0){
				if(board_match_shape(b, x, y, linear4_open_empty, linear4_open_occupied, linear4_open_alternate)){
					return 0;
				}
				// if(board_match_shape(b, x, y, dogleg4_up_empty, dogleg4_up_occupied, dogleg4_up_alternate)){
				// 	return 0;
				// }
				// if(board_match_shape(b, x, y, dogleg4_down_empty, dogleg4_down_occupied, NULL)){
				// 	return 0;
				// }
				// if(board_match_shape(b, x, y, snake_up_empty, snake_up_occupied, NULL)){
				// 	// printf("snake %d %d\n", x, y);
				// 	return 0;
				// }
				// the square is symmetrical
				// if(board_match(b, x, y, square4_empty, square4_occupied, NULL, 0)){
				// 	return 0;
				// }
			}
		}
	}

	return 1;
}

void
board_clear_regions(board *b){
	b->region_count = 0;
	memset(b->region_id, 0, b->width * b->height * sizeof(b->region_id[0]));
}

int
board_solvable(board *b){
	if(b->optimisations == 0) return 1;

	if(b->no_symmetry){
		if(board_position_occupied(b, 0, 0) && board_position_occupied(b, b->width - 1, b->height -1) &&
		   board_get_piece(b, 0, 0) > board_get_piece(b, b->width - 1, b->height - 1)) return 0;
	}

	if(b->optimisations >= 2){
		board_clear_regions(b);

		for(int y = 0, h = b->height; y < h; y++){
			for(int x = 0, w = b->width; x < w; x++){
				if(board_get_piece(b, x, y) == 0 && board_get_region(b, x, y) == 0){
					b->region_size[++b->region_count] = 0;

					board_region_propagate(b, x, y, b->region_count);

					if(b->region_size[b->region_count] < 4) return 0;
					if(b->optimisations >= 4) if(b->region_size[b->region_count] == 7) return 0;

					if(board_match_shape(b, x, y, linear4_open_empty, linear4_open_occupied, linear4_open_alternate)){
						return 0;
					}

				}
			}
		}

		if(b->optimisations >= 3){
			if(b->debug){
				printf("%2d: ", b->region_count);
				for(int region = 1, end = b->region_count; region <= end; region++){
					printf("%2d  ", b->region_size[region]);
				}
				printf("\n");
			}

			for(int region = 1, end = b->region_count; region <= end; region++){
				if(b->region_size[region] >= 4 && b->region_size[region] <= 6) {
					if(board_solvable_custom(b) == 0){
						return 0;
					}
					break;
				}
			}
		}
	}

	return 1;
}

void
board_write(board *b){
	for(int y = 0; y < b->height; y++){
		for(int x = 0; x < b->width; x++){
			int piece_id = board_get_piece(b, x, y) - 1;
			if(piece_id >= 0){
				piece *p = &b->pieces[piece_id];
				fprintf(b->output_fp, "%s", p->abbreviation);
			}else{
				fprintf(b->output_fp, "-");
			}
		}
	}

//	fprintf(b->output_fp, " %010d\n", b->solutions);
	fprintf(b->output_fp, "\n");

	if(b->solutions % 100 == 0) fflush(b->output_fp);
}

int
board_position_is_hole(board *b, int x, int y){
	if(board_position_occupied(b, x, y)) return 0;
	if(board_position_occupied(b, x - 1, y) == 0) return 0;
	if(board_position_occupied(b, x + 1, y) == 0) return 0;
	if(board_position_occupied(b, x, y - 1) == 0) return 0;
	if(board_position_occupied(b, x, y + 1) == 0) return 0;
	return 1;
}

int
board_valid_piece_position(board *b, piece *p, orientation *ori, int x, int y){
	ForEachPosition(pp, ori){
		int xx = x + ori->position[pp][0];
		int yy = y + ori->position[pp][1];
		if(board_get_piece(b, xx, yy) != 0) return 0;
	}

	return 1;
}

int
board_place_piece(board *b, piece *p, orientation *ori, int x, int y){
	ForEachPosition(pp, ori){
		int xx = x + ori->position[pp][0];
		int yy = y + ori->position[pp][1];
		board_set_piece(b, xx, yy, p->id);
	}

	ForEachPosition(pp, ori){
		int xx = x + ori->position[pp][0];
		int yy = y + ori->position[pp][1];
		if(board_position_is_hole(b, xx - 1, yy) ||
		   board_position_is_hole(b, xx + 1, yy) ||
		   board_position_is_hole(b, xx, yy + 1) ||
		   board_position_is_hole(b, xx, yy + 1)){
			return 0;
		}
	}

	return 1;
}

int
board_remove_piece(board *b, piece *p, orientation *ori, int x, int y){
	ForEachPosition(pp, ori){
		int xx = x + ori->position[pp][0];
		int yy = y + ori->position[pp][1];
		board_set_piece(b, xx, yy, 0);
	}
	return 0;
}

board *
board_new(int w, int h, piece *p){
	board *b = calloc(1, sizeof(board));
	b->width = w;
	b->height = h;
	b->piece_id = calloc(b->width * b->height, sizeof(b->piece_id[0]));
	b->region_id = calloc(b->width * b->height, sizeof(b->region_id[0]));
	b->print_frequency = 1000;
	b->terminate = INT32_MAX;
	b->pieces = p;
	b->advanced = 1;
	b->optimisations = 10;
	return b;
}

int
board_solve_propagate(board *b, int depth){
	if(b->debug) board_print(b);
	
	// b->search_calls[depth]++;

	if(depth == LEN(pieces)){
		if(b->optimisations == 0 || board_solved(b)){
			b->solutions++;

			if(b->print_frequency && (b->solutions % b->print_frequency) == 0){
				printf("solution %d\n", b->solutions);
				board_print(b);
			}

			if(b->output_fp) board_write(b);

			if(b->solutions >= b->terminate) exit(0);
		}else{
			b->invalid_solutions++;
		}
	}else{
		piece *p = pieces + depth;

		if(p->inactive){
			board_solve_propagate(b, depth + 1);
		}else{

			for(int y = 0, h = b->height - 1; y < h; y++){
				for(int x = 0, w = b->width - 1; x < w; x++){
					if(board_get_piece(b, x, y) != 0 &&
					   board_get_piece(b, x + 1, y) != 0 &&
					   board_get_piece(b, x, y + 1) != 0){
						continue;
					}
					ForEachOrientation(ori, p){
						if(x >= b->width - ori->width + 1) continue;
						if(y >= b->height - ori->height + 1) continue;
						if(board_valid_piece_position(b, p, ori, x, y)){
							if(board_place_piece(b, p, ori, x, y)){

								if(board_solvable(b)){
									// if(depth == 0) printf("prop at %d %d\n", x, y);
									board_solve_propagate(b, depth + 1);
								}else if(b->debug){
									printf("rejected\n");
									board_print(b);
								}
							}
							board_remove_piece(b, p, ori, x, y);
						}
					}
				}
			}
			// ForEachOrientation(ori, p){
			// 	// if(depth == 0) printf("depth 0 orientation %ld\n", ori - p->orientation);
			// 	for(int y = 0, h = b->height - ori->height + 1; y < h; y++){
			// 		for(int x = 0, w = b->width - ori->width + 1; x < w; x++){

			// 			if(board_valid_piece_position(b, p, ori, x, y)){
			// 				if(board_place_piece(b, p, ori, x, y)){

			// 					if(board_solvable(b)){
			// 						// if(depth == 0) printf("prop at %d %d\n", x, y);
			// 						board_solve_propagate(b, depth + 1);
			// 					}else if(b->debug){
			// 						printf("rejected\n");
			// 						board_print(b);
			// 					}
			// 				}
			// 				board_remove_piece(b, p, ori, x, y);
			// 			}
			// 		}
			// 	}
			// }
		}
	}

	return 0;
}

int
board_solve(board *b){
	board_solve_propagate(b, 0);

	if(b->verbose){
		for(int i = 0; i <= LEN(pieces) + 1; i++){
			printf("%2d %12lld\n", i, b->search_calls[i]);
		}
	}

	return 0;
}

void
board_disable_pieces(board *b, char *abbrevs){
	for(int i = 0; i < strlen(abbrevs); i++){
		piece *p = board_get_piece_from_abbreviation(b, abbrevs[i]);
		if(p){
			printf("disabling %s %c id %d\n", p->name, abbrevs[i], p->id);
			p->inactive = 1;
		}else{
			printf("no such piece as %c\nexiting\n", abbrevs[i]);
			exit(6);
		}

	}
}

void
board_print_pieces(board *b){
	printf("piece information, piece name, abbreviation and terminal colour code\n\n");
	ForEachPiece(p){
		for(int r = 0; r < 2; r++){
			if(r == 0) printf("%2d %-12s ", p->id, p->name);
			else printf("   %1s %2s         ", p->abbreviation, p->colour);
			for(int l = 0; l < LEN(p->layout); l++){
				char *s = p->layout[l][r];
				printf("%s  ", s);
			}
			printf("\n");
		}
		printf("\n");	
	}
	exit(0);
}

void
board_usage(){
	fprintf(stderr, "\
usage: iqfit [options]\n\
\n\
solve problems from the iqfit puzzle\n\
  -p n       print every n'th solution                              (default: 1000)\n\
  -t n       terminate after the n'th solution                      (default: none)\n\
  -d         show every step of solving the board                   (default: off)\n\
  -On        optimisation level 0-4                                 (default: 10!)\n\
  -o file    print all solutions to file                            (default: none)\n\
  -i file    read solutions from the specified file                 (default: none)\n\
  -S file    solve examples from the specified file                 (default: none)\n\
  -D abbrevs deactivate the pieces specified by their abbreviations (default: none)\n\
  -s         only generate the non-symmetry related solutions\n\
  -P         print description of the IQ FIT pieces\n\
  -v         print some information during processing\n\
  -h         print this message\n\
\n\
default behaviour is to enumerate all solutions, using all optimisations\n\
\n\
  -O0        disable all optimisations, and allow solutions that include gaps\n\
  -O1        generate all solutions that don't include any gaps\n\
  -O2        skip partial solutions that include 1, 2 or 3 size gaps\n\
  -O3        skip partial solutions that include some impossible shapes\n\
  -O4        skip partial solutions that include size 7 gaps\n\
\n\
Copyright (C) Mike Hartshorn, 2024\n");

	exit(1);
}

int
board_parse_args(board *b, int argc, char **argv){
	int c;

	while ((c = getopt(argc, argv, "D:sO:i:o:S:p:t:dv0hP")) != -1) {
		switch (c) {
		case 'i': b->input_filename = optarg; break;
		case 'o': b->output_filename = optarg; break;
		case 'S': b->solve_filename = optarg; break;
		case 'p': b->print_frequency = atoi(optarg); break;
		case 't': b->terminate = atoi(optarg); break;
		case 'd': b->debug++; break;
		case 'v': b->verbose++; break;
		case 'a': b->advanced = 1 - b->advanced; break;
		case 's': b->no_symmetry = 1 - b->no_symmetry; break;
		case 'O': b->optimisations = atoi(optarg); break;
		case 'D': board_disable_pieces(b, optarg); break;
		case 'P': board_print_pieces(b); break;
		case 'h': board_usage(); break;
		default: board_usage(); break;
		}
	}

	if(b->output_filename)
		b->output_fp = board_fopen(b->output_filename, "w");

	return 0;
}

void
board_process_solution(board *b, char *solution){
	int pos = 0;
	for(int y = 0, h = b->height; y < h; y++){
		for(int x = 0, w = b->width; x < w; x++){
			char c = solution[pos++];
			piece *p = board_get_piece_from_abbreviation(b, c);
//			printf("abbrev %c id %d\n", c, p->id);
			b->region_pieces[x][y] |= (1 << p->id);
		}
	}
}

void
board_print_solutions(board *b){
	for(int y = 0, h = b->height; y < h; y++){
		for(int x = 0, w = b->width; x < w; x++){
			printf("%6d ", b->region_pieces[x][y]);
		}
		printf("\n");
	}
}

void
board_process_solutions(board *b){
	char line[4096];
	FILE *fp = board_fopen(b->input_filename, "rt");

	while(fgets(line, 4096, fp)){
		board_process_solution(b, line);
	}

	fclose(fp);

	board_print_solutions(b);
}

void
board_solve_examples(board *b){
	char line[4096];
	FILE *fp = board_fopen(b->solve_filename, "rt");

	b->print_frequency = 1;

	while(fgets(line, 4096, fp)){
		board_reset(b);
		if(strncmp(line, "exit", 4) == 0) break;

		if(strlen(line) > 50){
			for(int x = 0, w = b->width * b->height; x < w; x++){
				char c = line[x];
				piece *p = board_get_piece_from_abbreviation(b, c);
				if(p){
					board_set_piece(b, x % b->width, x / b->width, p->id);
					p->inactive = 1;
				}else if(c != '_' && c != '-' && c != ' '){
					printf("invalid piece: %c\nexiting\n", c);
					exit(4);
				}
			}
		}else{

			printf("%s", line);
			for(int y = 0, h = b->height; y < h; y++){
				if(fgets(line, 4096, fp) == NULL){
					printf("incomplete puzzle\nexiting\n");
					exit(3);
				}

				for(int x = 0, w = b->width; x < w; x++){
					char c = line[x];
					piece *p = board_get_piece_from_abbreviation(b, c);
					if(p){
						board_set_piece(b, x, y, p->id);
						p->inactive = 1;
					}else if(c != '_' && c != '-' && c != ' '){
						printf("invalid piece: %c\nexiting\n", c);
						exit(4);
					}
				}
			}
		}

		board_print(b);

		board_solve(b);

		printf("--------------------\n");
	}

	fclose(fp);
}

int
main(int argc, char **argv) {

	board *b = board_new(BOARD_WIDTH, BOARD_HEIGHT, pieces);

	initialise_pieces(b);

	board_parse_args(b, argc, argv);

	if(b->input_filename){
		board_process_solutions(b);
	}else if(b->solve_filename){
		board_solve_examples(b);
	}else{
		board_solve(b);

		printf("total solutions %d, invalid solutions %d\n", b->solutions, b->invalid_solutions);
	}


	free(b);
}
