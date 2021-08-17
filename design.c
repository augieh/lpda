/*
 * design.c
 */

/*
 * Author: Augie Hansen ("Gus" -- KB0YH)
 * Copyright (C) 1992 Omniware
 * All rights reserved
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>
#include "lpda.h"

/* constants used in the optimum sigma calculation */
#define K1	0.243
#define K2	0.051

double
OptSigma(tau)
double tau;
{
	double opt_sigma;

	opt_sigma = K1 * tau - K2;
	return opt_sigma;
}

int
Design(dp)
LPDA *dp;
{
	USHORT rc;
	double low, high;
	double last_low, last_high, last_tau, last_sigma, last_ldr;
	FILE *fp;
	char line[NBYTES];

	/* read in design parameters from last design */
	fp = fopen("last.par", "r");
	if (fp != NULL) {
		/* data file exists -- read in the values */
		fgets(line, NBYTES, fp);
		last_low = atof(line);

		fgets(line, NBYTES, fp);
		last_high = atof(line);

		fgets(line, NBYTES, fp);
		last_tau = atof(line);

		fgets(line, NBYTES, fp);
		last_sigma = atof(line);

		fgets(line, NBYTES, fp);
		last_ldr = atof(line);

		fclose(fp);
	}
	else
		/* no data file (not an error) -- use dummy values */
		last_low = last_high = last_tau = last_sigma = 0.0;

	/* gather antenna design parameters from the user */
	/* low-frequency limit */
	low = 0.150;
	high = DBL_MAX;
	while (( rc = GetDouble("Low-frequency limit (MHz)", &dp->low,
		low, high, last_low)) != RC_SUCCESS)
	{
		switch (rc) {
		case RC_QUIT:
			return RC_QUIT;
		case RC_RANGE:
			fprintf(stderr, "\aRange error: ");
			fprintf(stderr, "low limit = %lf\n", low);
			continue;
		}
		break;
	}

	/* high-frequency limit */
	low = dp->low;
	high = DBL_MAX;
	while ((rc = GetDouble("High-frequency (MHz)", &dp->high,
		low, high, last_high)) != RC_SUCCESS)
	{
		switch (rc) {
		case RC_QUIT:
			return RC_QUIT;
		case RC_RANGE:
			fprintf(stderr, "\aRange error: ");
			fprintf(stderr, "high must exceed low\n");
			continue;
		}
		break;
	}

	/* taper factor */
	low = 0.7;
	high = 1.0;
	while ((rc = GetDouble("Taper factor", &dp->tau,
		low, high, last_tau)) != RC_SUCCESS)
	{
		switch (rc) {
		case RC_QUIT:
			return RC_QUIT;
		case RC_RANGE:
			fprintf(stderr, "\aRange error: ");
			fprintf(stderr, "%.2lf <= Tau <= %.2lf\n",
				low, high);
			continue;
		}
		break;
	}

	/* relative spacing factor */
	low = 0.04;
	high = 0.22;
	printf("Optimum sigma = %.3lf\n", OptSigma(dp->tau));
	while ((rc = GetDouble("Relative-spacing factor", &dp->sigma,
		low, high, last_sigma)) != RC_SUCCESS)
	{
		switch (rc) {
		case RC_QUIT:
			return RC_QUIT;
		case RC_RANGE:
			fprintf(stderr, "\aRange error: ");
			fprintf(stderr, "%.3lf <= Sigma <= %.3lf\n",
				low, high);
			continue;
		}
		break;
	}

	/* length-to-diameter ratio */
	low = 50.0;
	high = 10000.0;
	while ((rc = GetDouble("Length-to-diameter ratio", &dp->ldr,
		low, high, last_ldr)) != RC_SUCCESS)
	{
		switch (rc) {
		case RC_QUIT:
			return RC_QUIT;
		case RC_RANGE:
			fprintf(stderr, "\aRange error: ");
			fprintf(stderr, "%.3lf <= Length-to-diameter <= %.3lf\n",
				low, high);
			continue;
		}
		break;
	}

	/* save design parameters for this design */
	fp = fopen("last.par", "w");
	if (fp != NULL) {
		fprintf(fp, "%lf\n%lf\n%lf\n%lf\n%lf\n",
			dp->low, dp->high, dp->tau, dp->sigma, dp->ldr);
		fclose(fp);
	}

	return 0;
}
