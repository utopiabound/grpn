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
/* complex.h  by Paul Wilkins */
/* Here we set up the complex numbers */

#ifndef __COMPLEX_H
#define __COMPLEX_H

#include "typedefs.h"

struct _Cmplx {
   Real *re; 
   Real *im;
};


Cmplx * newCmplx();
void freeCmplx(Cmplx *);

char * printCmplx(Cmplx *);
char * printCmplxShort(Cmplx *);

Cmplx * inputCmplxReal(Cmplx *, Real *, Real *);
Cmplx * setCmplxReal(Cmplx *, Real *, Real *);
Cmplx * setCmplxCmplx(Cmplx *, Cmplx *);

Real * absCmplx(Cmplx *);
Real * thetaCmplx(Cmplx *);

Cmplx * polarCmplx(Cmplx *);
Cmplx * rectCmplx(Cmplx *);

Cmplx * negCmplx(Cmplx *);
Cmplx * negEqCmplx(Cmplx *);
Cmplx * invCmplx(Cmplx *);
Cmplx * invEqCmplx(Cmplx *);

Cmplx * powCmplxInt(Cmplx *, int);
Cmplx * powCmplx(Cmplx *, Cmplx *);
Cmplx * powCmplxReal(Cmplx *, Real *);
Cmplx * powRealCmplx(Real *, Cmplx *);

Cmplx * lnCmplx(Cmplx *);
Cmplx * logCmplx(Cmplx *);
Cmplx * expCmplx(Cmplx *);
Cmplx * expEqCmplx(Cmplx *);

Cmplx * sinCmplx(Cmplx *);
Cmplx * cosCmplx(Cmplx *);
Cmplx * tanCmplx(Cmplx *);
Cmplx * asinCmplx(Cmplx *);
Cmplx * acosCmplx(Cmplx *);
Cmplx * atanCmplx(Cmplx *);

Cmplx * mulCmplx(Cmplx *, Cmplx *);
Cmplx * mulCmplxReal(Cmplx *, Real *);

Cmplx * divCmplx(Cmplx *, Cmplx *);
Cmplx * divCmplxReal(Cmplx *, Real *);
Cmplx * divRealCmplx(Real *, Cmplx *);

Cmplx * addCmplx(Cmplx *, Cmplx *);
Cmplx * addCmplxReal(Cmplx *, Real *);

Cmplx * subCmplx(Cmplx *, Cmplx *);
Cmplx * subCmplxReal(Cmplx *, Real *);
Cmplx * subRealCmplx(Real *a, Cmplx *b);



#endif
