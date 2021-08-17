/*
 * fatal.c
 */

/*
 * Author: Augie Hansen ("Gus" -- KB0YH)
 * Copyright (C) 1992 Omniware
 * All rights reserved
 */

#include <stdio.h>
#include <stdlib.h>

void
FatalError(mesg)
char *mesg;
{
	fprintf(stderr, "%s\n", mesg);
	exit(1);
}
