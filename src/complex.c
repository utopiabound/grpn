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
/* complex.c  by Paul Wilkins */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "complex.h"
#include "real.h"
#include "number.h"
#include "constant.h"
#include "mode.h"

#include "lcd.h"  /* we need to know the display size */



/* create a new complex number */
Cmplx * newCmplx(){
   Cmplx *p;
   if(NULL == (p = (Cmplx *)malloc(sizeof(Cmplx)))){
      perror("Malloc");
      exit(0);
   }
   p->re = NULL;
   p->im = NULL;

   return p;
}

void freeCmplx(Cmplx *a){
   if(a){
      if(a->re) freeReal(a->re);
      if(a->im) freeReal(a->im);
      free((char *)a);
      a = NULL; /* make sure we die... */
   }
}

/* assumes data in in rectangular form */
Cmplx * setCmplxReal(Cmplx *a, Real *rp, Real *ip){
   if(a == NULL || rp == NULL || ip == NULL)
      { fprintf(stderr, "setCmplxReal(NULL)\n"); exit(0); }

   if(a->re) free((char *)a->re);
   if(a->im) free((char *)a->im);

   a->re = setRealReal(newReal(), rp);
   a->im = setRealReal(newReal(), ip);

   return a;
}

/* assumes data in in form defined by getRadixMode() and getPolarMode() */
Cmplx * inputCmplxReal(Cmplx *a, Real *rp, Real *ip){
   Cmplx *c1, *c2;
   if(a == NULL || rp == NULL || ip == NULL)
      { fprintf(stderr, "setCmplxReal(NULL)\n"); exit(0); }

   if(a->re) free((char *)a->re);
   if(a->im) free((char *)a->im);

   if(getPolarMode() == POLAR){
      c1 = newCmplx();
      c1->re = setRealReal(newReal(), rp);

      if(getRadixMode() == DEGREES) c1->im = divReal(ip, real180Pi);
      else c1->im = setRealReal(newReal(), ip);

      c2 = rectCmplx(c1);
      setCmplxCmplx(a, c2);
      freeCmplx(c1);
      freeCmplx(c2);
   } else {
      a->re = setRealReal(newReal(), rp);
      a->im = setRealReal(newReal(), ip);
   }

   return a;
}

Cmplx * setCmplxCmplx(Cmplx *a, Cmplx *b){
   if(a == NULL || b == NULL)
      { fprintf(stderr, "setCmplxCmplx(NULL)\n"); exit(0); }

   if(a->re) free((char *)a->re);
   if(a->im) free((char *)a->im);

   a->re = setRealReal(newReal(), b->re);
   a->im = setRealReal(newReal(), b->im);

   return a;
}

#define COMPLEX_PRINT_SIZE 90

char * printCmplxShort(Cmplx *a){
   char *c;
   char *p1;
   char *p2;
   Cmplx *c1;

   if(NULL == (c=(char *)malloc(COMPLEX_PRINT_SIZE)))
      { perror("Malloc"); exit(0); }

   if(getPolarMode() == POLAR){
      c1 = polarCmplx(a);
      p1 = printReal(c1->re);
      if(getRadixMode() == DEGREES){
         mulEqReal(c1->im, real180Pi);
         p2 = printReal(c1->im);
      } else {
         p2 = printReal(c1->im);
      }
      freeCmplx(c1);
   } else {
      p1 = printReal(a->re);
      p2 = printReal(a->im);
   }

   sprintf(c, "(%s, %s)", p1, p2);
   free(p1);
   free(p2);

   return c;
}

char * printCmplx(Cmplx *a){
   char *c;
   char *p1;
   char *p2;
   Cmplx *c1;

   if(NULL == (c=(char *)malloc(COMPLEX_PRINT_SIZE)))
      { perror("Malloc"); exit(0); }
   *c = '\0';

   if(getPolarMode() == POLAR){
      c1 = polarCmplx(a);
      p1 = printReal(c1->re);
      if(getRadixMode() == DEGREES){
         mulEqReal(c1->im, real180Pi);
         p2 = printReal(c1->im);
      } else {
         p2 = printReal(c1->im);
      }
      freeCmplx(c1);
   } else {
      p1 = printReal(a->re);
      p2 = printReal(a->im);
   }

   sprintf(c, "(%s, %s)", p1, p2);

   if(strlen(p1)+strlen(p2)+4 > lcdWidth-4) *(c+strlen(p1)+2) = '\n';
   free(p1);
   free(p2);

   return c;
}

Cmplx * negCmplx(Cmplx *a){
   Cmplx *p = newCmplx();

   p->re = negReal(a->re);
   p->im = negReal(a->im);

   return p;
}

Cmplx * negEqCmplx(Cmplx *a){
   negEqReal(a->re);
   negEqReal(a->im);

   return a;
}

/* calculate 1/(re+im) */
Cmplx * invCmplx(Cmplx *a){
   Real *r1, *r2, *r3;
   Cmplx *p = newCmplx();

   /* r1 = re^2 + im^2 */
   r1 = mulReal(a->re, a->re);
   r2 = mulReal(a->im, a->im);
   addEqReal(r1, r2);

   p->re = divReal(a->re, r1);

   r3 = divReal(a->im, r1);
   p->im = negEqReal(r3);

   freeReal(r1);
   freeReal(r2);

   return p;
}

/* calculate 1/(re+im) */
Cmplx * invEqCmplx(Cmplx *a){
   Real *r1, *r2;

   /* r1 = re^2 + im^2 */
   r1 = mulReal(a->re, a->re);
   r2 = mulReal(a->im, a->im);
   addEqReal(r1, r2);

   divEqReal(a->re, r1);

   divEqReal(a->im, r1);
   negEqReal(a->im);

   freeReal(r1);
   freeReal(r2);

   return a;
}

Cmplx * powCmplxInt(Cmplx *a, int b){
   int i;
   Cmplx *c1, *c2;

   c1 = setCmplxCmplx(newCmplx(), a);;

   for(i=1; i<b; i++){
      c2 = mulCmplx(c1, a);
      freeCmplx(c1);
      c1 = c2;
   }

   return c1;
}

Cmplx * powCmplx(Cmplx *a, Cmplx *b){
   Cmplx *c1;
   Cmplx *p;

   c1 = lnCmplx(a);

   p = expEqCmplx(mulCmplx(c1, b));

   freeCmplx(c1);

   return p;
}

Cmplx * powCmplxReal(Cmplx *a, Real *b){
   Cmplx *c1, *c2;
   Cmplx *p;

   c1 = lnCmplx(a);
   c2 = mulCmplxReal(c1, b);
   freeCmplx(c1);

   p = expCmplx(c2);
   freeCmplx(c2);

   return p;
}

Cmplx * powRealCmplx(Real *a, Cmplx *b){
   Real *r1;
   Cmplx *c1, *c2, *c3;
   Cmplx *p;

   if(-1 == cmpReal(a, realZero)){
      /* we can't take lnReal(<0) */
      c2 = setCmplxReal(newCmplx(), a, realZero);
      c3 = lnCmplx(c2);
      c1 = mulCmplx(b, c3);
      freeCmplx(c2);
      freeCmplx(c3);
   } else {
      r1 = lnReal(a);
      c1 = mulCmplxReal(b, r1);
      freeReal(r1);
   }

   p = expCmplx(c1);
   freeCmplx(c1);

   return p;
}

Cmplx * polarCmplx(Cmplx *a){
   Cmplx *p = newCmplx();

   p->re = absCmplx(a);
   p->im = thetaCmplx(a);

   return p;
}

Real * absCmplx(Cmplx *a){
   Real *re, *ri;
   Real *r1, *r2;

   /* Implements re = re^2 + im^2 but without the range problem */

   re = absReal(a->re);
   ri = absReal(a->im);

   if(1 == cmpReal(re, ri)){
      r2 = re;  /* bigger */
      r1 = ri;  /* smaller */
   } else {
      r2 = ri;
      r1 = re;
   }

   divEqReal(r1, r2);
   mulEqReal(r1, r1);
   addEqReal(r1, realOne);
   powEqReal(r1, realHalf);
   mulEqReal(r1, r2);

   freeReal(r2);

   return r1;

}

Real * thetaCmplx(Cmplx *a){
   int sign_re;
   Real *r1, *r2;
   Real *theta;

   r2 = atanEqReal(divReal(a->im, a->re));

   sign_re = cmpReal(a->re, realZero);

   if(0 == sign_re){

      /* -90 deg */
      if(-1 == cmpReal(a->im, realZero))
         theta = setRealDouble(newReal(), -M_PI/2.0);
      else /* 90 deg */
         theta = setRealDouble(newReal(), M_PI/2.0);

      freeReal(r2);
   }
   /* quadrant 2 and 3 */
   else if(-1 == sign_re){

      /* quad 3 */
      if(-1 == cmpReal(a->im, realZero))
         theta = subEqReal(r2, realPi);
      else /* quad 2 */
         theta = addEqReal(r2, realPi);

   }
   /* quadrant 1 and 4 */
   else{
      theta = r2;
   }

   return theta;
}

Cmplx * rectCmplx(Cmplx *a){
   Real *r1, *r2;
   Cmplx *p = newCmplx();

   r1 = cosReal(a->im);
   r2 = sinReal(a->im);
   p->re = mulEqReal(r1, a->re);
   p->im = mulEqReal(r2, a->re);

   return p;
}

Cmplx * lnCmplx(Cmplx *a){
   Cmplx *p;

   p = polarCmplx(a);

   lnEqReal(p->re);

   return p;

}

/* Note: instead of doing lnCmplx(a)/lnReal(10.0) I did it 
 *   like this.  Hopefully this way is a little faster???
 */
Cmplx * logCmplx(Cmplx *a){
   Real *r1;
   Cmplx *p;

   p = polarCmplx(a);

   r1 = lnReal(realTen);

   logEqReal(p->re);
   divEqReal(p->im, r1);

   freeReal(r1);

   return p;

}

Cmplx * expCmplx(Cmplx *a){
   Real *rr, *ri, *re;
   Cmplx *p = newCmplx();

   rr = cosReal(a->im);
   ri = sinReal(a->im);

   re = expReal(a->re);

   p->re = mulEqReal(rr, re);
   p->im = mulEqReal(ri, re);

   freeReal(re);

   return p;
}

Cmplx * expEqCmplx(Cmplx *a){
   Real *r1, *r2;

   r1 = expReal(a->re);

   /* res.re = e^re * cos(im) */
   mulEqReal(expEqReal(a->re), (r2=cosReal(a->im)));

   /* res.im = e^re * sin(im) */
   mulEqReal(sinEqReal(a->im), r1);

   freeReal(r1);
   freeReal(r2);

   return a;
}


/***************** TRIG *************************/

Cmplx * sinCmplx(Cmplx *a){
   Real *r1;
   Cmplx *p;
   Cmplx *c1, *c2, *c3, *c4, *c5;

   c1 = mulCmplx(cmplxI, a);
   c2 = expCmplx(c1);
   negEqCmplx(c1);
   c4 = expCmplx(c1);

   c5 = subCmplx(c2, c4);

   freeCmplx(c1);
   freeCmplx(c2);
   freeCmplx(c4);

   r1 = setRealDouble(newReal(), 2.0);
   c1 = setCmplxReal(newCmplx(), realZero, r1);
   freeReal(r1);

   p = divCmplx(c5, c1);

   freeCmplx(c1);
   freeCmplx(c5);
   
   return p;
}

Cmplx * cosCmplx(Cmplx *a){
   Cmplx *p;
   Cmplx *c1, *c2, *c4;

   c1 = mulCmplx(cmplxI, a);
   c2 = expCmplx(c1);
   negEqCmplx(c1);
   c4 = expCmplx(c1);

   p = addCmplx(c2, c4);

   freeCmplx(c1);
   freeCmplx(c2);
   freeCmplx(c4);

   mulEqReal(p->re, realHalf);
   mulEqReal(p->im, realHalf);

   return p;
}

Cmplx * tanCmplx(Cmplx *a){
   Cmplx *p;
   Cmplx *c1, *c2;

   c1 = sinCmplx(a);
   c2 = cosCmplx(a);

   p = divCmplx(c1, c2);

   freeCmplx(c1);
   freeCmplx(c2);

   return p;
}

Cmplx * asinCmplx(Cmplx *a){
   Real *r1;
   Cmplx *z, *sqzp1, *sqzm1;
   Cmplx *c1, *c2, *c3, *c4, *c5, *c6, *c7;

   z = newCmplx();
   setCmplxCmplx(z, a);
   if(1 == cmpReal(realZero, a->re)) negEqCmplx(z);

   /* sqrt(z + 1) */
   c1 = addCmplxReal(z, realOne);
   sqzp1 = powCmplxReal(c1, realHalf);
   freeCmplx(c1);

   /* sqrt(z - 1) */
   c1 = subCmplxReal(z, realOne);
   sqzm1 = powCmplxReal(c1, realHalf);
   freeCmplx(c1);

   /* if imag_part(sqzp1) < 0.0 then sqzp1 = -sqzp1 */
   if(1 == cmpReal(realZero, sqzp1->im)) negEqCmplx(sqzp1);

   c2 = mulCmplx(sqzp1, sqzm1);
   c3 = addCmplx(c2, z);
   c4 = lnCmplx(c3);
   c5 = mulCmplx(c4, cmplxI);

   c6 = subRealCmplx(realPi2, c5);

   if(1 == cmpReal(c6->re, realPi2)){
      c7 = subRealCmplx(realPi, c6);
      freeCmplx(c6);
      c6 = c7;
   }
   r1 = negReal(realPi);
   if(-1 == cmpReal(c6->re, r1)){
      c7 = subRealCmplx(r1, c6);
      freeCmplx(c6);
      c6 = c7;
   }
   freeReal(r1);
   if(1 == cmpReal(realZero, a->re)) negEqCmplx(c6);
      

   freeCmplx(sqzp1);
   freeCmplx(sqzm1);
   freeCmplx(z);
   freeCmplx(c2);
   freeCmplx(c3);
   freeCmplx(c4);
   freeCmplx(c5);
   
   return c6;
}

Cmplx * acosCmplx(Cmplx *a){
   Cmplx *p, *c1;

   c1 = asinCmplx(a);
   p = subRealCmplx(realPi2, c1);
   freeCmplx(c1);

   return p;
}

Cmplx * atanCmplx(Cmplx *a){
   Real *r, *x, *y, *rsq;
   Real *r1, *r2, *r3, *r4, *r5, *r6, *r7;
   Cmplx *p = newCmplx();

   r = absCmplx(a);
   x = a->re;
   y = a->im;
   rsq = mulReal(r, r);
   freeReal(r);

   r1 = mulReal(x, realTwo);
   r2 = subReal(realOne, rsq);
   r3 = atan2Real(r1, r2);
   p->re = mulReal(r3, realHalf);
   freeReal(r1);
   freeReal(r2);
   freeReal(r3);

   r1 = mulReal(y, realTwo);
   r2 = addReal(rsq, realOne);
   r3 = addReal(r2, r1);
   r4 = subReal(r2, r1);
   r5 = divReal(r3, r4);
   r6 = lnReal(r5);
   r7 = mulReal(realHalf, realHalf);
   p->im = mulReal(r6, r7);
   freeReal(r1);
   freeReal(r2);
   freeReal(r3);
   freeReal(r4);
   freeReal(r5);
   freeReal(r6);
   freeReal(r7);
   freeReal(rsq);


   return p; 
}


/*************** MULTIPLY **********************/

/* multiply 2 Cmplx numbers */
Cmplx * mulCmplx(Cmplx *a, Cmplx *b){
   Real *r1, *r2, *r3, *r4, *r5, *r6;
   Cmplx *p = newCmplx();

   r1 = mulReal(a->re, b->re);
   r2 = mulReal(a->im, b->im);

   r3 = mulReal(a->re, b->im);
   r4 = mulReal(a->im, b->re);

   p->re = subEqReal(r1, r2);
   p->im = addEqReal(r3, r4);

   freeReal(r2);
   freeReal(r4);

   return p;
}

/* multiply a Cmplx by a Real number */
Cmplx * mulCmplxReal(Cmplx *a, Real *b){
   Cmplx *p = newCmplx();

   p->re = mulReal(a->re, b);
   p->im = mulReal(a->im, b);

   return p;
}

/***************** DIVIDE ***********************/
   

/* divide 2 Cmplx numbers */
Cmplx * divCmplx(Cmplx *a, Cmplx *b){
   Real *r2, *r3;
   Real *r5, *r6;
   Real *r8, *r9;
   Cmplx *p = newCmplx();

   /* r3 = bre^2 + bim^2 */
   r3 = mulReal(b->re, b->re);
   r2 = mulReal(b->im, b->im);
   addEqReal(r3, r2);

   /* r6 = bre*are + bim*aim */
   r6 = mulReal(b->re, a->re);
   r5 = mulReal(b->im, a->im);
   addEqReal(r6, r5);

   /* r9 = bre*aim - bim*are */
   r9 = mulReal(b->re, a->im);
   r8 = mulReal(b->im, a->re);
   subEqReal(r9, r8);

   p->re = divEqReal(r6, r3);
   p->im = divEqReal(r9, r3);

   freeReal(r2); freeReal(r3);
   freeReal(r5); 
   freeReal(r8); 

   return p;
}

/* divide a Cmplx by a Real number */
Cmplx * divCmplxReal(Cmplx *a, Real *b){
   Cmplx *p = newCmplx();

   p->re = divReal(a->re, b);
   p->im = divReal(a->im, b);

   return p;
}

/* divide a Real by a Cmplx number */
Cmplx * divRealCmplx(Real *a, Cmplx *b){
   Real *r2, *r3, *r4, *r5, *r6;
   Cmplx *p = newCmplx();

   /* r3 = bre^2 + bim^2 */
   r3 = mulReal(b->re, b->re);
   r2 = mulReal(b->im, b->im);
   addEqReal(r3, r2);

   r4 = divReal(b->re, r3);
   r5 = divReal(b->im, r3);

   r6 = negReal(a);

   p->re = mulEqReal(r4, a);
   p->im = mulEqReal(r5, r6);

   freeReal(r2);
   freeReal(r3);
   freeReal(r6);

   return p;
}






/***************** ADD ***************************/


/* add 2 Cmplx numbers */
Cmplx * addCmplx(Cmplx *a, Cmplx *b){
   Cmplx *p = newCmplx();

   p->re = addReal(a->re, b->re);
   p->im = addReal(a->im, b->im);

   return p;
}

/* add a Cmplx and a Real number */
Cmplx * addCmplxReal(Cmplx *a, Real *b){
   Cmplx *p = newCmplx();

   p->re = addReal(a->re, b);
   p->im = setRealReal(newReal(), a->im);

   return p;
}


/******************* SUBTRACT *******************/


/* subtract 2 Cmplx numbers */
Cmplx * subCmplx(Cmplx *a, Cmplx *b){
   Cmplx *p = newCmplx();

   p->re = subReal(a->re, b->re);
   p->im = subReal(a->im, b->im);

   return p;
}

/* subtract a Real from a Cmplx */
Cmplx * subCmplxReal(Cmplx *a, Real *b){
   Cmplx *p = newCmplx();

   p->re = subReal(a->re, b);
   p->im = setRealReal(newReal(), a->im);

   return p;
}

/* subtract a Cmplx from a Real */
Cmplx * subRealCmplx(Real *a, Cmplx *b){
   Cmplx *p = newCmplx();

   p->re = subReal(a, b->re);
   p->im = negReal(b->im);

   return p;
}

