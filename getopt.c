/*
 * This is a version of the public domain getopt implementation by
 * Henry Spencer originally posted to net.sources.
 *
 * This file is in the public domain.
 */

#include <stdio.h>
#include <string.h>

#define getopt xgetopt
#define optarg xoptarg
#define optind xoptind

char *optarg; /* Global argument pointer. */
int optind = 0; /* Global argv index. */

static char *scan = NULL; /* Private scan pointer. */

int
getopt(int argc, char *argv[], char *optstring)
{
	char c;
	char *place;

	optarg = NULL;

	if (!scan || *scan == '\0') {
		if (optind == 0)
			optind++;

		if (optind >= argc || argv[optind][0] != '-' || argv[optind][1] == '\0')
			return EOF;
		if (argv[optind][1] == '-' && argv[optind][2] == '\0') {
			optind++;
			return EOF;
		}

		scan = argv[optind]+1;
		optind++;
	}

	c = *scan++;
	place = strchr(optstring, c);

	if (!place || c == ':') {
		fprintf(stderr, "unknown option -%c\n", c);
		return '?';
	}

	place++;
	if (*place == ':') {
		if (*scan != '\0') {
			optarg = scan;
			scan = NULL;
		} else if( optind < argc ) {
			optarg = argv[optind];
			optind++;
		} else {
			fprintf(stderr, "option requires argument -%c\n", c);
			return ':';
		}
	}

	return c;
}

