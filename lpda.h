/* lpda.h */

/*
 * Author: Augie Hansen ("Gus" -- KB0YH)
 * Copyright (C) 1992 Omniware
 * All rights reserved
 */

#define MAXDNAME	13
#define NBYTES		30
#define FRAC_INCH	5
#define EOS		'\0'
#if defined (MSDOS)
#define MAXNAME		8
#define PATHSEP		'\\'
#else
#define MAXNAME		14
#define PATHSEP		'/'
#endif

typedef unsigned short USHORT;
typedef enum { FALSE, TRUE } BOOL;

/* design and construction data */
typedef struct lpda_st {
	/* design parameters */
	double low, high;       /* frequency limits */
	double tau;		/* taper factor */
	double sigma;		/* relative spacing constant */
	double ldr;		/* target length-to-diameter ratio */

	/* analysis variables */
	double sigma_prime;
	int N;                  /* number of elements */
	double L;		/* boom length */
	double B, Bar, Bs;      /* bandwidth factors */
	double *element;	/* pointer to element length array */
	double *spacing;	/* pointer to spacing array */
	double pos;		/* relative position on boom */
	double Z0, Zav, Zt;	/* feeder system impedances */
	double R0;		/* desired feed-point impedance */
} LPDA;

/* functions return codes */
#define RC_SUCCESS	0
#define RC_FAILURE	1
#define RC_NOINPUT	2
#define RC_RANGE	3
#define RC_QUIT		4

/* function prototypes */
void Usage(char *);
char *GetPgmName(char *, char *);
void Clean(char *);
int Design(LPDA *);
void FatalError(char *);
char *English(double, char *);
USHORT GetDouble(char *, double *, double, double, double);
void CheckArgs(int, char **, char *, char *);
int Analyze(LPDA *);
int Report(LPDA *, char *);
char *EraseNL(char *);
