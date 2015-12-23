/*
 * Copyright (C) 2006-2011  Trevor Woerner
 */

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <libgen.h>
void *mempcpy (void *dest, const void *src, size_t n);

#include "config.h"

#define LINESZ 4096

/* ------------------------------------------- */
/* prototypes                                  */
/* ------------------------------------------- */
static unsigned get_colour (double percentage);
static void usage (const char *cmd_p);
static void process_cmdline_args (int argc, char *argv[]);
static int find_largest_count (FILE *inFile_p);
static void process_code (char *inBuf_p, char *outBuf_p, unsigned sz);

/* ------------------------------------------- */
/* globals                                     */
/* ------------------------------------------- */
static char *inFileName_pG = NULL;
static char *outFileName_pG = NULL;

/* ------------------------------------------- */
/* meat and potatoes                           */
/* ------------------------------------------- */
int
main (int argc, char *argv[])
{
	char line[LINESZ];
	FILE *inFile_p, *outFile_p;
	char *ptr;
	int count, largestCount;
	unsigned lineNum;
	char code[LINESZ], newcode[LINESZ];
	int ret;
	double coverage;
	unsigned colour;

	process_cmdline_args (argc, argv);

	// open files
	inFile_p = fopen (inFileName_pG, "r");
	if (inFile_p == NULL) {
		perror ("fopen (inFile)");
		return 1;
	}
	outFile_p = fopen (outFileName_pG, "w");
	if (outFile_p == NULL) {
		perror ("fopen (outFile)");
		return 1;
	}

	largestCount = find_largest_count (inFile_p);

	// start output file
	fprintf (outFile_p, "<html>\n");
	fprintf (outFile_p, "<head><title>%s</title></head>\n", outFileName_pG);
	fprintf (outFile_p, "<body style=\"padding: 2em 1em 2em 110px; background-position: top left; background-attachment: fixed; background-repeat: no-repeat; background-image: url(gcov2html-check.png);\" bgcolor=\"#000000\"><pre>\n");

	// parse through gcov file
	// to create output
	while (fgets (line, LINESZ, inFile_p) != NULL) {
		if (strncmp (line, "function", 8) == 0)
			continue;
		if (strncmp (line, "call", 4) == 0)
			continue;
		if (strncmp (line, "branch", 6) == 0)
			continue;

		// get count
		ret = sscanf (line, " %d", &count);
		if (ret == 0) {
			if (strncmp (line, "    #####", 9) == 0)
				count = 0;
			else
				count = -1;
		}

		// get line number
		// skip ':'
		ptr = strstr (line, ":");
		++ptr;
		sscanf (ptr, " %u", &lineNum);

		// get rest of line (code)
		// skip ':'
		ptr = strstr (ptr, ":");
		++ptr;
		strcpy (code, ptr);

		// remove trailing '\n'
		ptr = code;
		while ((*ptr != '\n') && *ptr) ++ptr;
		*ptr = 0;

		// calculate colour
		if (count == -1)
			colour = 0xffffff;
		else {
			coverage = (double)count / (double)largestCount;
			colour = get_colour (coverage);
		}
		process_code (code, newcode, LINESZ);
		fprintf (outFile_p, "<font color=\"#%06x\">%s</font>\n", colour, newcode);
	}

	// end output file
	fprintf (outFile_p, "</pre></body>\n");
	fprintf (outFile_p, "</html>");

	// done
	free (inFileName_pG);
	free (outFileName_pG);
	fclose (outFile_p);
	fclose (inFile_p);

	return 0;
}

/* ------------------------------------------- */
/* private helpers                             */
/* ------------------------------------------- */
static unsigned
get_colour (double percentage)
{
	if (percentage == 0.0)
		return 0x808080;
	if (percentage < .1)
		return 0x9933ff;
	if (percentage < .2)
		return 0x33ffff;
	if (percentage < .3)
		return 0x3399ff;
	if (percentage < .4)
		return 0x3333ff;
	if (percentage < .5)
		return 0x00ff00;
	if (percentage < .6)
		return 0x008000;
	if (percentage < .7)
		return 0xffff33;
	if (percentage < .8)
		return 0xff9933;
	if (percentage < .9)
		return 0xff00ff;
	return 0xff3333;
}

static int
find_largest_count (FILE *inFile_p)
{
	char line[LINESZ];
	int count, largestCount;

	/* -- preconds -- (begin) -- */
	if (inFile_p == NULL)
		return 0;
	/* -- preconds --  (end)  -- */

	// parse through gcov file
	// to find the largest count
	// thankfully the "Runs:" line comes early
	largestCount = 0;
	while (fgets (line, LINESZ, inFile_p) != NULL) {
		if (strncmp (line, "function", 8) == 0)
			continue;
		if (strncmp (line, "call", 4) == 0)
			continue;
		if (strncmp (line, "branch", 6) == 0)
			continue;

		// get count
		sscanf (line, " %d", &count);
		if (count > largestCount)
			largestCount = count;
	}

	rewind (inFile_p);
	return largestCount;
}

static void
process_code (char *inBuf_p, char *outBuf_p, unsigned sz)
{
	unsigned i = 0;

	/* -- preconds -- (begin) -- */
	if (sz == 0)
		return;
	if (inBuf_p == NULL)
		return;
	if (outBuf_p == NULL)
		return;
	/* -- preconds --  (end)  -- */

	--sz;
	while (*inBuf_p) {
		switch (*inBuf_p) {
			case '&':
				if ((i + 5) <= sz) {
					outBuf_p = mempcpy (outBuf_p, "&amp;", 5);
					i += 5;
				}
				else {
					*outBuf_p = 0;
					return;
				}
				break;

			case '<':
				if ((i + 4) <= sz) {
					outBuf_p = mempcpy (outBuf_p, "&lt;", 4);
					i += 4;
				}
				else {
					*outBuf_p = 0;
					return;
				}
				break;

			default:
				*outBuf_p++ = *inBuf_p;
				++i;
				break;
		}

		++inBuf_p;

		if (i >= sz) {
			*outBuf_p = 0;
			return;
		}
	}

	*outBuf_p = 0;
}

static void
process_cmdline_args (int argc, char *argv[])
{
	int c;
	char *ptr;
	struct option longOpts[] = {
		{"help", no_argument, NULL, 'h'},
		{"out", required_argument, NULL, 'o'},
		{NULL, 0, NULL, 0},
	};

	for (;;) {
		c = getopt_long (argc, argv, "ho:", longOpts, NULL);
		if (c == -1)
			break;

		switch (c) {
			case 'h':
				usage (argv[0]);
				exit (0);

			case 'o':
				outFileName_pG = strdup (optarg);
				if (outFileName_pG == NULL) {
					perror ("strdup()");
					exit (1);
				}
				break;
		}
	}

	if ((optind + 1) == argc) {
		// check input filename
		ptr = strstr (argv[optind], ".gcov");
		if (ptr == NULL) {
			fprintf (stderr, "input filename doesn't end in '.gcov', exiting\n");
			exit (1);
		}
		inFileName_pG = strdup (argv[optind]);

		// form name for output file (if output name not specified)
		if (outFileName_pG == NULL) {
			outFileName_pG = (char*)malloc (strlen (basename (argv[optind])) + 10);
			if (outFileName_pG == NULL) {
				perror ("malloc (outFileName)");
				exit (1);
			}
			memset (outFileName_pG, 0, sizeof (*outFileName_pG));
			memcpy (outFileName_pG, basename (argv[optind]),
					strlen (basename (argv[optind])));
			ptr = strstr (outFileName_pG, ".gcov");
			if (ptr == NULL) {
				fprintf (stderr, "input filename doesn't contain '.gcov'?!\n");
				exit (1);
			}
			memcpy (ptr, ".html", 5);
		}
	}
	else {
		fprintf (stderr, "required argument <gcov file> not specified\n\n");
		usage (argv[0]);
		exit (1);
	}
}

static void
usage (const char *cmd_p)
{
	printf ("%s\n", PACKAGE_STRING);
	printf ("\n");
	printf ("USAGE: %s [options] <gcov file>\n", cmd_p);
	printf ("  where:\n");
	printf ("    --help | -h          This usage information.\n");
	printf ("    --out  | -o <file>   Send output to <file> instead of calculated filename.\n");
	printf ("\n");
	printf ("bugs, issues, requests, problems? %s\n", PACKAGE_BUGREPORT);
}
