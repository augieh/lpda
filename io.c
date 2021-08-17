/*
 * io.c
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
#include <ctype.h>
#include "lpda.h"

char *
English(len, buf)
double len;
char *buf;
{
	double feet, inches, delta, fraction;
	char frac_inch[FRAC_INCH];		/* text buffer */
	int n;					/* characters written */

	/* calculate feet, inches, and fractional inches */
	feet = floor(len);			/* whole feet */
	delta = len - feet;			/* fraction of a foot */
	inches = delta * 12.0;			/* converted to inches */
	fraction = inches - floor(inches);	/* fraction of an inch */

	/* round off to the nearst 1/8 inch for display purposes */
	strcpy(frac_inch, "");
	if (fraction >= 0.0625)
		strcpy(frac_inch, "1/8");
	if (fraction >= 0.1875)
		strcpy(frac_inch, "1/4");
	if (fraction >= 0.3125)
		strcpy(frac_inch, "3/8");
	if (fraction >= 0.4375)
		strcpy(frac_inch, "1/2");
	if (fraction >= 0.5625)
		strcpy(frac_inch, "5/8");
	if (fraction >= 0.6875)
		strcpy(frac_inch, "3/4");
	if (fraction >= 0.8125)
		strcpy(frac_inch, "7/8");
	if (fraction >= 0.9375) {
		strcpy(frac_inch, "");
		inches += 1.0;
	}

	/* adjust for round-off that produces a foot or more */
	if (inches > 12.0) {
		inches -= 12.0;
		feet += 1.0;
	}

	/* fill buffer with the length string */
	n = 0;
	if ((int)feet > 0)
		n += sprintf(buf, "%d\'", (int)feet);
	if ((int)inches > 0) {
		n += sprintf(buf + n, "%d", (int)inches);
		if (strlen(frac_inch) > 0)
			n += sprintf(buf + n, "-%s", frac_inch);
		sprintf(buf + n, "\"");
	}
	else if (strlen(frac_inch) > 0)
		sprintf(buf + n, "%s\"", frac_inch);

	return buf;
} /* end English() */

USHORT
GetDouble(prompt, varptr, low, high, last)
char *prompt;		/* prompt string */
double *varptr;		/* pointer to the receiving variable */
double low, high;	/* acceptable range of input */
double last;		/* previous value */
{
	char line[NBYTES];
	USHORT rc = RC_SUCCESS;

	/* prompt user for input and read it */
	printf("%s [%.3lf]: ", prompt, last);
	fgets(line, NBYTES, stdin);
	EraseNL(line);

	if (*line == '\0')
		/* no input -- use previous value */
		*varptr = last;
	else if (!isdigit(*line) && *line != '.')
		/* user interrupt -- quit program */
		return RC_QUIT;
	else
		/* convert input string to the required form */
		*varptr = atof(line);

	/* validate input */
	if (*varptr < low || *varptr > high)
		rc = RC_RANGE;

	return rc;
} /* end GetDouble() */

char *
EraseNL(s)
char *s;
{
	char *cp;

	/* find the NL code, if any, or EOS */
	cp = s;
	while (*cp != '\0' && *cp != '\n')
		++cp;

	/* overwrite with a null byte */
	*cp = '\0';

	return s;
} /* end EraseNL() */
