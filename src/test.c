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
/* test.c  by Paul Wilkins 3/15/97 */

#include <stdio.h>
#include <stdlib.h>
#include "test.h"

#include "number.h"
#include "complex.h"
#include "real.h"
#include "matrix.h"
#include "funcs.h"

int main() {

   char *c;
   int i, j;
   Number *a, *b;
   Real *ra, *rb;
   Cmplx *ca, *cb;
   Matrix *ma, *mb;
   Number *p1;

   printf("test()\n");

   ra = setRealDouble(newReal(), 3.0);
   rb = setRealDouble(newReal(), 9.0);

   ca = setCmplxReal(newCmplx(), 
      setRealDouble(newReal(), 3.0), 
      setRealDouble(newReal(), 2.0));

   cb = setCmplxReal(newCmplx(), 
      setRealDouble(newReal(), 0.0), 
      setRealDouble(newReal(), 0.0));

   ma = newMatrix();
   /*
   setMatrix(ma, setNumberCmplx(newNumber(), ca), 0, 0);
   setMatrix(ma, setNumberCmplx(newNumber(), cb), 0, 1);
   setMatrix(ma, setNumberCmplx(newNumber(), cb), 1, 0);
   setMatrix(ma, setNumberCmplx(newNumber(), ca), 1, 1);
   */
   setMatrix(ma, setNumberReal(newNumber(), ra), 0, 0);
   setMatrix(ma, setNumberReal(newNumber(), rb), 0, 1);
   setMatrix(ma, setNumberReal(newNumber(), rb), 0, 2);
   setMatrix(ma, setNumberReal(newNumber(), rb), 1, 0);
   setMatrix(ma, setNumberCmplx(newNumber(), ca), 1, 1);
   setMatrix(ma, setNumberReal(newNumber(), ra), 1, 2);

   mb = newMatrix();
   setMatrix(mb, setNumberReal(newNumber(), ra), 0, 0);
   setMatrix(mb, setNumberReal(newNumber(), ra), 0, 1);
   setMatrix(mb, setNumberReal(newNumber(), ra), 1, 0);
   setMatrix(mb, setNumberReal(newNumber(), ra), 1, 1);
   setMatrix(mb, setNumberReal(newNumber(), rb), 2, 0);
   setMatrix(mb, setNumberReal(newNumber(), rb), 2, 1);

   /* a = setNumberReal(newNumber(), ra);  */
   /* a = setNumberCmplx(newNumber(), ca); */
   a = setNumberMatrix(newNumber(), ma);
   /* b = setNumberReal(newNumber(), ra); */
   /* b = setNumberCmplx(newNumber(), cb);  */
   b = setNumberMatrix(newNumber(), mb); 


   c = printNumber(a);
   printf("a = %s\n", c); 
   free(c);

   c = printNumber(b);
   printf("b = %s\n", c); 
   free(c);

   p1 = negNumber(a);
   c = printNumber(p1);
   printf("-a = %s\n", c); 
   free(c);
   freeNumber(p1);

   p1 = mulNumber(a, b);
   if(p1){
      c = printNumber(p1);
      printf("a * b = %s\n", c); 
      free(c);
      freeNumber(p1);
   }

   p1 = addNumber(a, b);
   if(p1){
      c = printNumber(p1);
      printf("a + b = %s\n", c); 
      free(c);
      freeNumber(p1);
   }

   p1 = subNumber(a, b);
   if(p1){
      c = printNumber(p1);
      printf("a - b = %s\n", c); 
      free(c);
      freeNumber(p1);
   }

   p1 = subNumber(b, a);
   if(p1){
      c = printNumber(p1);
      printf("b - a = %s\n", c); 
      free(c);
      freeNumber(p1);
   }

   p1 = divNumber(a, b);
   if(p1){
      c = printNumber(p1);
      printf("a / b = %s\n", c); 
      free(c);
      freeNumber(p1);
   }

   p1 = divNumber(b, a);
   if(p1){
      c = printNumber(p1);
      printf("b / a = %s\n", c); 
      free(c);
      freeNumber(p1);
   }

   p1 = modNumber(a, b);
   if(p1){
      c = printNumber(p1);
      printf("a mod b = %s\n", c); 
      free(c);
      freeNumber(p1);
   }

   p1 = modNumber(b, a);
   if(p1){
      c = printNumber(p1);
      printf("b mod a = %s\n", c); 
      free(c);
      freeNumber(p1);
   }


   printf("done testing...\n");
   return 0;
}

