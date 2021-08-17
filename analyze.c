/* 
 * analyze.c
 */

/*
 * Author: Augie Hansen ("Gus" -- KB0YH)
 * Copyright (C) 1992 Omniware
 * All rights reserved
 */

#include <stdlib.h>
#include <math.h>
#include "lpda.h"

int
Analyze(dp)
LPDA *dp;
{
	int n;			/* loop counter */
	double delta;		/* partial element length */
	double rawN;		/* calculated N before rounding */
	double cot_alpha;       /* intermediate result */

	/* calculate bandwidth and angle factors */
	dp->B = dp->high / dp->low;
	cot_alpha = (4 * dp->sigma) / (1 - dp->tau);
	dp->Bar = 1.1 + 7.7 * (1 - dp->tau) *
		(1 - dp->tau) * cot_alpha;
	dp->Bs = dp->B * dp->Bar;

	/* calculate boom length and number of elements */
	dp->L = (984 / dp->low) * (.25 * (1 - (1 / dp->Bs)) *
		cot_alpha);
	rawN = 1 + (log(dp->Bs) / log(1 / dp->tau));
	delta = rawN - floor(rawN);
	dp->N = (int)((delta > 0.3) ? ceil(rawN) : floor(rawN));

	/* calculate element lengths */
	dp->element = (double *)malloc(dp->N * sizeof (double));
	dp->element[0] = 492 / dp->low;
	for (n = 1; n < dp->N; ++n)
		dp->element[n] = dp->element[n - 1] * dp->tau;

	/* calculate inter-element spacings */
	dp->spacing = (double *)malloc((dp->N - 1) * sizeof (double));
	dp->spacing[0] = .5 * (dp->element[0] - dp->element[1]) *
		cot_alpha;
	for (n = 1; n < dp->N - 1; ++n)
		dp->spacing[n] = dp->spacing[n - 1] * dp->tau;

	/* calculate feeder system base values */
	dp->Zt = (984 / dp->low) / 8;
	dp->sigma_prime = dp->sigma / sqrt(dp->tau);
	dp->Zav = 120 * (log(dp->ldr) - 2.25);

	return 0;
}
