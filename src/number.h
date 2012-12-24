/*

Copyright (C) 2002  Paul Wilkins

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

*/
/* number.h  by Paul Wilkins 3/15/97 */

#ifndef __NUMBER_H
#define __NUMBER_H

#include "typedefs.h"

/* types of numbers */
#define REAL    0x1
#define COMPLEX 0x2
#define MATRIX  0x4


/* a number */
struct _Number {
   int refCnt;   /* refference count. used to implement undo */
   int type;     /* the type of number data pointe to */
   void *data;   /* the data (Real, complex, etc.) */
};

#include "real.h"
#include "complex.h"
#include "matrix.h"

Number * newNumber();
void freeNumber(Number *);

void clrRefcntNumber(Number *);
void incRefcntNumber(Number *);
void decRefcntNumber(Number *);

Number * setNumberReal(Number *, Real *);
Number * setNumberCmplx(Number *, Cmplx *);
Number * setNumberMatrix(Number *, Matrix *);
Number * setNumberNumber(Number *, Number *);

char * printNumber(Number *);
char * printNumberShort(Number *);

Number * negNumber(Number *);
Number * invNumber(Number *);

Number * lnNumber(Number *);
Number * logNumber(Number *);
Number * expNumber(Number *);

Number * powNumber(Number *, Number *);

Number * dbNumber(Number *, double);
Number * ripNumber(Number *, Number *);

Number * sinNumber(Number *);
Number * cosNumber(Number *);
Number * tanNumber(Number *);
Number * asinNumber(Number *);
Number * acosNumber(Number *);
Number * atanNumber(Number *);

Number * mulNumber(Number *, Number *);
Number * divNumber(Number *, Number *);
Number * addNumber(Number *, Number *);
Number * subNumber(Number *, Number *);

#endif

