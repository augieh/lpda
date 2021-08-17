/*
 * lpda.c
 *
 * LPDA design program
 */

/*
 * Author: Augie Hansen ("Gus" -- KB0YH)
 * Copyright (C) 1992 Omniware
 * All rights reserved
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "lpda.h"

int
main(argc, argv)
int argc;
char *argv[];
{
	static char pgm[MAXNAME + 1] = "lpda";
	static char dfile[MAXDNAME + 1] = "lpda.dat";
	LPDA data;		/* antenna data */

#if defined (MSDOS)
	if (_osmajor >= 3)
		GetPgmName(pgm, argv[0]);
#else
	GetPgmName(pgm, argv[0]);
#endif
	CheckArgs(argc, argv, dfile, pgm);
	if (Design(&data) == RC_QUIT)
		Clean(pgm);
	Analyze(&data);
	Report(&data, dfile);

	return 0;
} /* end main() */

void
CheckArgs(ac, av, filename, pname)
int ac;
char **av;
char *filename;
char *pname;
{
	/* check command-line syntax */
	switch (ac) {
	case 1:
		/* use built-in datafile name */
		break;
	case 2:
		/* use specified datafile */
		strcpy(filename, av[1]);
		break;
	default:
		Usage(pname);
		break;
	}
} /* end CheckArgs() */

void
Usage(pname)
char *pname;
{
	fprintf(stderr, "Usage: %s [data_file]\n", pname);
	exit(1);
} /* end usage() */

char *
GetPgmName(pname, path)
char *pname, *path;
{
	char *cp;
	cp = path;

	/* find the end of the path string */
	while (*cp != EOS)
		++cp;
	--cp;

#if defined (MSDOS)
	/* back up to start of the filename portion of the path */
	while (*cp != PATHSEP && *cp != ':' && cp > path) {
		--cp;
		if (*cp == '.')
			*cp = EOS;	/* drop the extension */
	}
	/* move off the separator */
	if (*cp == PATHSEP || *cp == ':')
		++cp;
#else
	/* back up to start of the filename portion of the path */
	while (*cp != PATHSEP && cp > path)
		--cp;
	/* move off the separator */
	if (*cp == PATHSEP)
		++cp;
#endif

	/* copy the name to the pname array */
	strcpy(pname, cp);

	return pname;
} /* end GetPgmName() */

void
Clean(pname)
char *pname;
{
	fprintf(stderr, "%s terminated by user command\n", pname);
	exit(0);
} /* end usage() */
