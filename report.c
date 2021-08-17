/*
 * report.c
 */

/*
 * Author: Augie Hansen ("Gus" -- KB0YH)
 * Copyright (C) 1992 Omniware
 * All rights reserved
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "lpda.h"

int
Report(dp, dfile)
LPDA *dp;
char *dfile;
{
	int n;			/* loop counter */
	FILE *fp;           	/* data-file pointer */
	char text[NBYTES];	/* general text buffer */
	double tradius;		/* turning radius */
	char el[NBYTES];	/* element length buffer */ 
	char ed[NBYTES];	/* element center diameter buffer */
	char bp[NBYTES];	/* element boom mounting position */
	char ies[NBYTES];	/* inter-element spacing buffer */
	double dia;		/* diameter (ft) at element center */
	double min_dia = 0.02;	/* diameter (ft) of smallest tip sections */
	double wavenum = 468;	/* conversion constant */

	/* save and print summary */
	fp = fopen(dfile, "w");
	if (fp == NULL)
		FatalError("Cannot open the datafile");
		
	fprintf(fp, "\n***** LPDA DESIGN SUMMARY *****\n");
	fprintf(fp, "Frequency range: %.3lf to %.3lf MHz\n",
		dp->low, dp->high);
	fprintf(fp, "B = %.2lf, Bar = %.2lf, Bs = %.2lf\n",
		dp->B, dp->Bar, dp->Bs);
	fprintf(fp, "Taper factor = %.3lf  Relative spacing = %.3lf\n",
		dp->tau, dp->sigma);
	fprintf(fp, "Target l/d Ratio = %d\n", (int)dp->ldr);
	
	fprintf(fp, "\n***** CONSTRUCTION SUMMARY *****\n");
	fprintf(fp, "%-8s%-8s%-13s%-20s%-10s\t%s\n", "Element",
		"Element", "Element", "Inter-element",
		"Boom", "Center");
	fprintf(fp, "%-8s%-8s%-13s%-20s%-10s\t%s\n", "Number",
		"Length", "Half-Len", "Spacing",
		"Position", "Diameter");
	dp->pos = 0.125;	/* allow for end-cap, etc. */

	/* first N - 1 elements (with spacings) */
	for (n = 0; n < dp->N - 1; ++n) {
		fprintf(fp, "%-8d%-8.2lf%-13s%-8.2lf%-12s%-10s",
			n + 1, dp->element[n], English(dp->element[n] /
			2, el), dp->spacing[n],
			English(dp->spacing[n], ies), English(dp->pos, bp));
		dia = ((dp->element[n] / dp->ldr) - min_dia) / 2 ;
		fprintf(fp, "\t%-6s\n", English(dia, ed));
		dp->pos += dp->spacing[n];
	}

	/* last element (no spacing value needed) */
	fprintf(fp, "%-8d%-8.2lf%-13s%20s%-10s", n + 1,
		dp->element[n], English(dp->element[n] / 2,
		el), "", English(dp->pos, bp));
	dia = ((dp->element[n] / dp->ldr) - min_dia) / 2 ;
	fprintf(fp, "\t%-6s\n", English(dia, ed));

	/* save boom length and turning radius */
	dp->L = dp->pos + 0.125;	/* end-cap adjustment */
	tradius = sqrt(pow(dp->L / 2, 2) + pow(dp->element[0] / 2, 2));
	fprintf(fp,"Boom length = %.2lf (%s)\n", dp->L, English(dp->L, text));
	fprintf(fp,"Worst-case turning radius = %.2lf (%s)\n",
		tradius, English(tradius, text));

	/* save feeder system values */
	fprintf(fp, "\n***** FEEDER SYSTEM SUMMARY *****\n");
	fprintf(fp, "Zav = %.1lf Ohms\n", dp->Zav);

	/* 208 Ohm feeder (for 52 Ohm coax to 1:4 balun) */
	dp->R0 = 208.0;
	dp->Z0 = (pow(dp->R0, 2) / (8 * dp->sigma_prime * dp->Zav)) +
		dp->R0 * sqrt(pow(dp->R0 / (8 * dp->sigma_prime *
		dp->Zav), 2) + 1);
	fprintf(fp, "Z0 = %.1lf Ohms for R0 = %.1lf (50 Ohm coax thru 4:1 balun)\n",
		dp->Z0, dp->R0);

	/* 75 Ohm feeder (direct) */
	dp->R0 = 75.0;
	dp->Z0 = (pow(dp->R0, 2) / (8 * dp->sigma_prime * dp->Zav)) +
		dp->R0 * sqrt(pow(dp->R0 / (8 * dp->sigma_prime *
		dp->Zav), 2) + 1);
	fprintf(fp, "Z0 = %.1lf Ohms for R0 = %.1lf (Direct 75 Ohm feed)\n\n",
		dp->Z0, dp->R0);
	fclose(fp);

	/* play back the data file for the user */
	if ((fp = fopen(dfile, "r")) == NULL)
		FatalError("Cannot open data file for reading");
	while (fgets(text, NBYTES, fp))
		fputs(text, stdout);
	fclose(fp);

	return 0;
}
