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
/* real.h  by Paul Wilkins */
/* Here we set up the real numbers */

#ifndef __REAL_H
#define __REAL_H

#include "typedefs.h"

// precision of diplayed real
// (internal is still DBL_DIG)
#define PRINT_REAL_DIG DBL_DIG-3 // = usuallly 12

#define REAL_INF 0x1
#define REAL_NAN 0x2
#define REAL_OK  0x4

struct _Real {
    char ok;
    double num;
};

Real * newReal();
void freeReal(Real *);

char * printReal(Real *);
Real * setRealDouble(Real *, double);
double setDoubleReal(Real *);
Real * setRealReal(Real *, Real *);

Real * fromRadixReal(Real *a);
Real * toRadixReal(Real *a);


int cmpReal(Real *, Real *);
int isIntReal(Real *);

Real * absReal(Real *);

Real * negReal(Real *);
Real * negEqReal(Real *);
Real * invReal(Real *);
Real * invEqReal(Real *);

Real * powRealInt(Real *a, int);
Real * powReal(Real *a, Real *b);
Real * powEqReal(Real *a, Real *b);

Real * lnReal(Real *a);
Real * lnEqReal(Real *a);
Real * logReal(Real *a);
Real * logEqReal(Real *a);
Real * expReal(Real *a);
Real * expEqReal(Real *a);

Real * sinReal(Real *a);
Real * sinEqReal(Real *a);
Real * cosReal(Real *a);
Real * cosEqReal(Real *a);
Real * tanReal(Real *a);
Real * tanEqReal(Real *a);

Real * asinReal(Real *a);
Real * asinEqReal(Real *a);
Real * acosReal(Real *a);
Real * acosEqReal(Real *a);
Real * atanReal(Real *a);
Real * atan2Real(Real *a, Real *b);
Real * atanEqReal(Real *a);

Real * mulReal(Real *, Real *);
Real * mulEqReal(Real *, Real *);
Real * divReal(Real *, Real *);
Real * divEqReal(Real *, Real *);
Real * modReal(Real *, Real *);
Real * addReal(Real *, Real *);
Real * addEqReal(Real *, Real *);
Real * subReal(Real *, Real *);
Real * subEqReal(Real *, Real *);

Real * lshiftReal(Real *, Real *);
Real * rshiftReal(Real *, Real *);

Real * bitwiseANDReal(Real *a, Real *b);
Real * bitwiseORReal(Real *a, Real *b);
Real * bitwiseXORReal(Real *a, Real *b);

#endif
