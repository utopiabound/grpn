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
/* number.c  by Paul Wilkins 3/15/97 */

#include <stdio.h>
#include <stdlib.h>

#include "number.h"
#include "matrix.h"
#include "complex.h"
#include "real.h"
#include "mode.h"
#include "constant.h"
#include "error.h"

Number * newNumber(){
   Number *p;
   if(NULL == (p = (Number *)malloc(sizeof(Number)))){
      perror("Malloc");
      exit(0);
   }
   p->refCnt = 0;
   return p;
}

void freeNumber(Number *a){
   if(a == NULL) return;

   if(a->refCnt != 0){
      return;
   }

   switch(a->type){
      case REAL:
         freeReal(a->data);
         break;
      case COMPLEX:
         freeCmplx(a->data);
         break;
      case MATRIX:
         freeMatrix(a->data);
         break;
      default:
         fprintf(stderr, "Error: Unknown number type\n");
         exit(0);
   }
   free(a);
}

void clrRefcntNumber(Number *a){
   if(a == NULL){ fprintf(stderr, "clrRefcntNumber(NULL)\n"); exit(0); }
   a->refCnt = 0;
}
void incRefcntNumber(Number *a){
   if(a == NULL){ fprintf(stderr, "incRefcntNumber(NULL)\n"); exit(0); }
   a->refCnt++;
}
void decRefcntNumber(Number *a){
   if(a == NULL){ fprintf(stderr, "decRefcntNumber(NULL)\n"); exit(0); }
   a->refCnt--;
}

char * printNumber(Number *a){
   if(a == NULL){ fprintf(stderr, "printNumber(NULL)\n"); exit(0); }
   switch(a->type){
      case REAL:
         return printReal(a->data);
         break;
      case COMPLEX:
         return printCmplx(a->data);
         break;
      case MATRIX:
         return printMatrix(a->data);
         break;
      default:
         fprintf(stderr, "Error: printNumber(): Unknown number type: 0x%x\n", a->type);
         exit(0);
   }
}

char * printNumberShort(Number *a){
   if(a == NULL){ fprintf(stderr, "printNumber(NULL)\n"); exit(0); }
   switch(a->type){
      case REAL:
         return printReal(a->data);
         break;
      case COMPLEX:
         return printCmplxShort(a->data);
         break;
      case MATRIX:
         return printMatrixShort(a->data);
         break;
      default:
         fprintf(stderr, "Error: printNumber(): Unknown number type\n");
         exit(0);
   }
}


Number * setNumberReal(Number *a, Real *b){
   a->type = REAL;
   a->data = setRealReal(newReal(), b);
   return a;
}

Number * setNumberCmplx(Number *a, Cmplx *b){
   a->type = COMPLEX;
   a->data = setCmplxCmplx(newCmplx(), b);
   return a;
}

Number * setNumberMatrix(Number *a, Matrix *b){
   a->type = MATRIX;
   a->data = setMatrixMatrix(newMatrix(), b);
   return a;
}

Number * setNumberNumber(Number *a, Number *b){
   a->type = b->type;
   switch(b->type){
      case REAL:
         a->data = setRealReal(newReal(), b->data);
         break;
      case COMPLEX:
         a->data = setCmplxCmplx(newCmplx(), b->data);
         break;
      case MATRIX:
         a->data = setMatrixMatrix(newMatrix(), b->data);
         break;
      default:
         fprintf(stderr, "Error: Unknown number type\n");
         exit(0);
   }
   return a;
}

Number * negNumber(Number *a){
   Number *c = newNumber();

   if(a == NULL){ fprintf(stderr, "negNumber(NULL)\n"); exit(0); }

   switch(a->type){
      case REAL:
         c->type = REAL;
         c->data = negReal(a->data);
         break;
      case COMPLEX:
         c->type = COMPLEX;
         c->data = negCmplx(a->data);
         break;
      case MATRIX:
         c->type = MATRIX;
         c->data = negMatrix(a->data);
         break;
      default:
         fprintf(stderr, "Error: Unknown number type\n");
         exit(0);
   }
   if(c->data == NULL){ free(c); c = NULL; }
   return c;
}

Number * invNumber(Number *a){
   Number *c = newNumber();

   if(a == NULL){ fprintf(stderr, "invNumber(NULL)\n"); exit(0); }

   switch(a->type){
      case REAL:
         c->type = REAL;
         c->data = invReal(a->data);
         break;
      case COMPLEX:
         c->type = COMPLEX;
         c->data = invCmplx(a->data);
         break;
      case MATRIX:
         c->type = MATRIX;
         c->data = invMatrix(a->data);
         break;
      default:
         fprintf(stderr, "Error: Unknown number type\n");
         exit(0);
   }
   if(c->data == NULL){ free(c); c = NULL; }
   return c;
}

Number * lnNumber(Number *a){
   Real *r1;
   Cmplx *c1;
   Number *c = newNumber();

   if(a == NULL){ fprintf(stderr, "lnNumber(NULL)\n"); exit(0); }

   switch(a->type){
      case REAL:
         if(-1 == cmpReal(a->data, realZero)){
            /* ln(<0) is complex ! */
            c1 = setCmplxReal(newCmplx(), a->data, realZero);
            c->type = COMPLEX;
            c->data = lnCmplx(c1);
            freeCmplx(c1);
         } else {
            c->type = REAL;
            c->data = lnReal(a->data);
         }
         break;
      case COMPLEX:
         c->type = COMPLEX;
         c->data = lnCmplx(a->data);
         break;
      case MATRIX:
         c->type = MATRIX;
         invalidTyprError("ln");
         c->data = NULL;
         break;
      default:
         fprintf(stderr, "Error: Unknown number type\n");
         exit(0);
   }
   if(c->data == NULL){ free(c); c = NULL; }
   return c;
}

Number * logNumber(Number *a){
   Real *r1;
   Cmplx *c1;
   Number *c = newNumber();

   if(a == NULL){ fprintf(stderr, "logNumber(NULL)\n"); exit(0); }


   switch(a->type){
      case REAL:
         if(-1 == cmpReal(a->data, realZero)){
            /* log(<0) is complex ! */
            c1 = setCmplxReal(newCmplx(), a->data, realZero);
            c->type = COMPLEX;
            c->data = logCmplx(c1);
            freeCmplx(c1);
         } else {
            c->type = REAL;
            c->data = logReal(a->data);
         }
         break;
      case COMPLEX:
         c->type = COMPLEX;
         c->data = logCmplx(a->data);
         break;
      case MATRIX:
         c->type = MATRIX;
         invalidTyprError("log");
         c->data = NULL;
         break;
      default:
         fprintf(stderr, "Error: Unknown number type\n");
         exit(0);
   }
   if(c->data == NULL){ free(c); c = NULL; }
   return c;
}

Number * expNumber(Number *a){
   Number *c = newNumber();

   if(a == NULL){ fprintf(stderr, "expNumber(NULL)\n"); exit(0); }

   switch(a->type){
      case REAL:
         c->type = REAL;
         c->data = expReal(a->data);
         break;
      case COMPLEX:
         c->type = COMPLEX;
         c->data = expCmplx(a->data);
         break;
      case MATRIX:
         c->type = MATRIX;
         invalidTyprError("exp");
         c->data = NULL;
         break;
      default:
         fprintf(stderr, "Error: Unknown number type\n");
         exit(0);
   }
   if(c->data == NULL){ free(c); c = NULL; }
   return c;
}


/* note that complex numbers take care of degree/radian conversions,
 * but we have to deal with that for real numbers here.
 */
Number * sinNumber(Number *a){
   Real *r1;
   Number *c = newNumber();

   if(a == NULL){ fprintf(stderr, "sin Number(NULL)\n"); exit(0); }

   switch(a->type){
      case REAL:
         c->type = REAL;
        
         r1 = fromRadixReal(a->data);
         c->data = sinReal(r1);
         freeReal(r1);

         break;
      case COMPLEX:
         c->type = COMPLEX;
         c->data = sinCmplx(a->data);
         break;
      case MATRIX:
         c->type = MATRIX;
         invalidTyprError("sin");
         c->data = NULL;
         break;
      default:
         fprintf(stderr, "Error: Unknown number type\n");
         exit(0);
   }
   if(c->data == NULL){ free(c); c = NULL; }
   return c;
}

Number * cosNumber(Number *a){
   Real *r1;
   Number *c = newNumber();

   if(a == NULL){ fprintf(stderr, "cos Number(NULL)\n"); exit(0); }

   switch(a->type){
      case REAL:
         c->type = REAL;

         r1 = fromRadixReal(a->data);
         c->data = cosReal(r1);
         freeReal(r1);

         break;
      case COMPLEX:
         c->type = COMPLEX;
         c->data = cosCmplx(a->data);
         break;
      case MATRIX:
         c->type = MATRIX;
         invalidTyprError("cos");
         c->data = NULL;
         break;
      default:
         fprintf(stderr, "Error: Unknown number type\n");
         exit(0);
   }
   if(c->data == NULL){ free(c); c = NULL; }
   return c;
}

Number * tanNumber(Number *a){
   Real *r1;
   Number *c = newNumber();

   if(a == NULL){ fprintf(stderr, "tan Number(NULL)\n"); exit(0); }

   switch(a->type){
      case REAL:
         c->type = REAL;

         r1 = fromRadixReal(a->data);
         c->data = tanReal(r1);
         freeReal(r1);

         break;
      case COMPLEX:
         c->type = COMPLEX;
         c->data = tanCmplx(a->data);
         break;
      case MATRIX:
         c->type = MATRIX;
         invalidTyprError("tan");
         c->data = NULL;
         break;
      default:
         fprintf(stderr, "Error: Unknown number type\n");
         exit(0);
   }
   if(c->data == NULL){ free(c); c = NULL; }
   return c;
}

Number * asinNumber(Number *a){
   Real *r1;
   Cmplx *c1;
   Number *c = newNumber();

   if(a == NULL){ fprintf(stderr, "asin Number(NULL)\n"); exit(0); }

   switch(a->type){
      case REAL:
         if(1 == cmpReal(a->data, realOne) ||
            -1 == cmpReal(a->data, realMOne)){
            /* asin(>1 or <-1) is complex ! */
            c1 = setCmplxReal(newCmplx(), a->data, realZero);
            c->type = COMPLEX;
            c->data = asinCmplx(c1);
            freeCmplx(c1);
         } else {
            c->type = REAL;

            r1 = asinReal(a->data);
            c->data = toRadixReal(r1);
            freeReal(r1);
         }
         break;
      case COMPLEX:
         c->type = COMPLEX;
         c->data = asinCmplx(a->data);
         break;
      case MATRIX:
         c->type = MATRIX;
         invalidTyprError("asin");
         c->data = NULL;
         break;
      default:
         fprintf(stderr, "Error: Unknown number type\n");
         exit(0);
   }
   if(c->data == NULL){ free(c); c = NULL; }
   return c;
}

Number * acosNumber(Number *a){
   Real *r1;
   Cmplx *c1;
   Number *c = newNumber();

   if(a == NULL){ fprintf(stderr, "acos Number(NULL)\n"); exit(0); }

   switch(a->type){
      case REAL:
         if(1 == cmpReal(a->data, realOne) ||
            -1 == cmpReal(a->data, realMOne)){
            /* acos(>1 or <-1) is complex ! */
            c1 = setCmplxReal(newCmplx(), a->data, realZero);
            c->type = COMPLEX;
            c->data = acosCmplx(c1);
            freeCmplx(c1);
         } else {
            c->type = REAL;

            r1 = acosReal(a->data);
            c->data = toRadixReal(r1);
            freeReal(r1);
         }
         break;
      case COMPLEX:
         c->type = COMPLEX;
         c->data = acosCmplx(a->data);
         break;
      case MATRIX:
         c->type = MATRIX;
         invalidTyprError("acos");
         c->data = NULL;
         break;
      default:
         fprintf(stderr, "Error: Unknown number type\n");
         exit(0);
   }
   if(c->data == NULL){ free(c); c = NULL; }
   return c;
}

Number * atanNumber(Number *a){
   Real *r1;
   Number *c = newNumber();

   if(a == NULL){ fprintf(stderr, "atan Number(NULL)\n"); exit(0); }

   switch(a->type){
      case REAL:
         c->type = REAL;
         r1 = atanReal(a->data);
         c->data = toRadixReal(r1);
         freeReal(r1);
         break;
      case COMPLEX:
         c->type = COMPLEX;
         c->data = atanCmplx(a->data);
         break;
      case MATRIX:
         c->type = MATRIX;
         invalidTyprError("atan");
         c->data = NULL;
         break;
      default:
         fprintf(stderr, "Error: Unknown number type\n");
         exit(0);
   }
   if(c->data == NULL){ free(c); c = NULL; }
   return c;
}


Number * powNumber(Number *a, Number *b){
   Cmplx *c1;
   Number *c = newNumber();

   if(a == NULL || b == NULL) 
      { fprintf(stderr, "powNumber(NULL)\n"); exit(0); }

   switch(a->type){
      case REAL:
         switch(b->type){
            case REAL:
               if(-1 == cmpReal(a->data, realZero) &&
                  0 == isIntReal(b->data)){
                  /* do complex math */
                  c->type = COMPLEX;
                  c1 = setCmplxReal(newCmplx(), a->data, realZero);
                  c->data = powCmplxReal(c1, b->data);
                  freeCmplx(c1);
               } else {
                  c->type = REAL;
                  c->data = powReal(a->data, b->data);
               }
               break;
            case COMPLEX:
               c->type = COMPLEX;
               if(-1 == cmpReal(a->data, realZero)){
                  c1 = setCmplxReal(newCmplx(), a->data, realZero);
                  c->data = powCmplx(c1, b->data);
                  freeCmplx(c1);
               } else {
                  c->data = powRealCmplx(a->data, b->data);
               }
               break;
            case MATRIX:
               invalidTyprError("pow");
               c->data = NULL;
               break;
            default:
               fprintf(stderr, "Error: Unknown number type\n");
               exit(0);
               break;
         }
         break;
      case COMPLEX:
         switch(b->type){
            case REAL:
               c->type = COMPLEX;
               c->data = powCmplxReal(a->data, b->data);
               break;
            case COMPLEX:
               c->type = COMPLEX;
               c->data = powCmplx(a->data, b->data);
               break;
            case MATRIX:
               invalidTyprError("pow");
               c->data = NULL;
               break;
            default:
               fprintf(stderr, "Error: Unknown number type\n");
               exit(0);
               break;
         }
         break;
      case MATRIX:
         invalidTyprError("pow");
         c->data = NULL;
         break;
      default:
         fprintf(stderr, "Error: Unknown number type\n");
         exit(0);
   }

   if(c->data == NULL){ free(c); c = NULL; }
   return c;
}

Number * ripNumber(Number *a, Number *b){
   Number *c;
   Number *num;
   Number *den;

   if(a == NULL || b == NULL) 
      { fprintf(stderr, "ripNumber(NULL)\n"); exit(0); }

   switch(a->type){
      case REAL:
      case COMPLEX:
         switch(b->type){
            case REAL:
            case COMPLEX:
               num = mulNumber(a,b);
               den = addNumber(a,b);
               c = divNumber(num,den);
               freeNumber(num);
               freeNumber(den);
               break;
            case MATRIX:
              invalidTyprError("rip");
              return NULL;
              break;
            default:
              fprintf(stderr, "Error: Unknown number type\n");
              exit(0);
              break;
         }
         break;
      case MATRIX:
        invalidTyprError("rip");
        return NULL;
        break;
      default:
        fprintf(stderr, "Error: Unknown number type\n");
        exit(0);
        break;
   }

   return c;
}


Number * dbNumber(Number *a, double sf){
   Real *r1;
   Number *c;
   Number *n1, *n2;

   if(a == NULL) 
      { fprintf(stderr, "dbNumber(NULL)\n"); exit(0); }

   switch(a->type){
      case REAL:
      case COMPLEX:
         n1 = logNumber(a);
         n2 = setNumberReal(newNumber(), setRealDouble((r1=newReal()), sf));
         c = mulNumber(n1, n2);
         freeNumber(n1);
         freeNumber(n2);
         freeReal(r1);
         break;
      case MATRIX:
        invalidTyprError("db");
        return NULL;
        break;
      default:
        fprintf(stderr, "Error: Unknown number type\n");
        exit(0);
        break;
   }

   return c;
}

Number * mulNumber(Number *a, Number *b){
   Number *c = newNumber();

   if(a == NULL || b == NULL) 
      { fprintf(stderr, "mulNumber(NULL)\n"); exit(0); }

   switch(a->type){
      case REAL:
         switch(b->type){
            case REAL:
               c->type = REAL;
               c->data = mulReal(a->data, b->data);
               break;
            case COMPLEX:
               c->type = COMPLEX;
               c->data = mulCmplxReal(b->data, a->data);
               break;
            case MATRIX:
               c->type = MATRIX;
               c->data = mulMatrixReal(b->data, a->data);
               break;
            default:
               fprintf(stderr, "Error: Unknown number type\n");
               exit(0);
               break;
         }
         break;
      case COMPLEX:
         switch(b->type){
            case COMPLEX:
               c->type = COMPLEX;
               c->data = mulCmplx(a->data, b->data);
               break;
            case REAL:
               c->type = COMPLEX;
               c->data = mulCmplxReal(a->data, b->data);
               break;
            case MATRIX:
               c->type = MATRIX;
               c->data = mulMatrixCmplx(b->data, a->data);
               break;
            default:
               fprintf(stderr, "Error: Unknown number type\n");
               exit(0);
               break;
         }
         break;
      case MATRIX:
         switch(b->type){
            case MATRIX:
               c->type = MATRIX;
               c->data = mulMatrix(a->data, b->data);
               c->type = MATRIX;
               break;
            case COMPLEX:
               c->type = MATRIX;
               c->data = mulMatrixCmplx(a->data, b->data);
               break;
            case REAL:
               c->type = MATRIX;
               c->data = mulMatrixReal(a->data, b->data);
               break;
            default:
               fprintf(stderr, "Error: Unknown number type\n");
               exit(0);
               break;
         }
         break;
      default:
         fprintf(stderr, "Error: Unknown number type\n");
         exit(0);
   }

   if(c->data == NULL){ free(c); c = NULL; }
   return c;
}


Number * divNumber(Number *a, Number *b){
   Number *c = newNumber();

   if(a == NULL || b == NULL) 
      { fprintf(stderr, "mulNumber(NULL)\n"); exit(0); }

   switch(a->type){
      case REAL:
         switch(b->type){
            case REAL:
               c->type = REAL;
               c->data = divReal(a->data, b->data);
               break;
            case COMPLEX:
               c->type = COMPLEX;
               c->data = divRealCmplx(a->data, b->data);
               break;
            case MATRIX:
               c->type = MATRIX;
               c->data = divRealMatrix(a->data, b->data);
               break;
            default:
               fprintf(stderr, "Error: Unknown number type\n");
               exit(0);
               break;
         }
         break;
      case COMPLEX:
         switch(b->type){
            case COMPLEX:
               c->type = COMPLEX;
               c->data = divCmplx(a->data, b->data);
               break;
            case REAL:
               c->type = COMPLEX;
               c->data = divCmplxReal(a->data, b->data);
               break;
            case MATRIX:
               c->type = MATRIX;
               c->data = divCmplxMatrix(a->data, b->data);
               break;
            default:
               fprintf(stderr, "Error: Unknown number type\n");
               exit(0);
               break;
         }
         break;
      case MATRIX:
         switch(b->type){
            case MATRIX:
               c->type = MATRIX;
               c->data = divMatrix(a->data, b->data);
               break;
            case COMPLEX:
               c->type = MATRIX;
               c->data = divMatrixCmplx(a->data, b->data);
               break;
            case REAL:
               c->type = MATRIX;
               c->data = divMatrixReal(a->data, b->data);
               break;
            default:
               fprintf(stderr, "Error: Unknown number type\n");
               exit(0);
               break;
         }
         break;
      default:
         fprintf(stderr, "Error: Unknown number type\n");
         exit(0);
   }

   if(c->data == NULL){ free(c); c = NULL; }
   return c;
}


Number * modNumber(Number *a, Number *b){
   Number *c = newNumber();

   if(a == NULL || b == NULL) 
      { fprintf(stderr, "modNumber(NULL)\n"); exit(0); }

   switch(a->type){
      case REAL:
         switch(b->type){
            case REAL:
               c->type = REAL;
               c->data = modReal(a->data, b->data);
               break;
            case COMPLEX:
               c->type = COMPLEX;
               invalidTyprError("mod");
               c->data = NULL;
               break;
            case MATRIX:
               c->type = MATRIX;
               invalidTyprError("mod");
               c->data = NULL;
               break;
            default:
               fprintf(stderr, "Error: Unknown number type\n");
               exit(0);
               break;
         }
         break;
      case COMPLEX:
         switch(b->type){
            case COMPLEX:
               c->type = COMPLEX;
               invalidTyprError("mod");
               c->data = NULL;
               break;
            case REAL:
               c->type = COMPLEX;
               invalidTyprError("mod");
               c->data = NULL;
               break;
            case MATRIX:
               c->type = MATRIX;
               invalidTyprError("mod");
               c->data = NULL;
               break;
            default:
               fprintf(stderr, "Error: Unknown number type\n");
               exit(0);
               break;
         }
         break;
      case MATRIX:
         switch(b->type){
            case MATRIX:
               c->type = MATRIX;
               invalidTyprError("mod");
               c->data = NULL;
               break;
            case COMPLEX:
               c->type = MATRIX;
               invalidTyprError("mod");
               c->data = NULL;
               break;
            case REAL:
               c->type = MATRIX;
               c->data = modMatrixReal(a->data, b->data);
               break;
            default:
               fprintf(stderr, "Error: Unknown number type\n");
               exit(0);
               break;
         }
         break;
      default:
         fprintf(stderr, "Error: Unknown number type\n");
         exit(0);
   }

   if(c->data == NULL){ free(c); c = NULL; }
   return c;
}


Number * addNumber(Number *a, Number *b){
   Number *c = newNumber();

   if(a == NULL || b == NULL) 
      { fprintf(stderr, "addNumber(NULL)\n"); exit(0); }

   switch(a->type){
      case REAL:
         switch(b->type){
            case REAL:
               c->type = REAL;
               c->data = addReal(a->data, b->data);
               break;
            case COMPLEX:
               c->type = COMPLEX;
               c->data = addCmplxReal(b->data, a->data);
               break;
            case MATRIX:
               c->type = MATRIX;
               c->data = addMatrixReal(b->data, a->data);
               break;
            default:
               fprintf(stderr, "Error: Unknown number type\n");
               exit(0);
               break;
         }
         break;
      case COMPLEX:
         switch(b->type){
            case COMPLEX:
               c->type = COMPLEX;
               c->data = addCmplx(a->data, b->data);
               break;
            case REAL:
               c->type = COMPLEX;
               c->data = addCmplxReal(a->data, b->data);
               break;
            case MATRIX:
               c->type = MATRIX;
               c->data = addMatrixCmplx(b->data, a->data);
               break;
            default:
               fprintf(stderr, "Error: Unknown number type\n");
               exit(0);
               break;
         }
         break;
      case MATRIX:
         switch(b->type){
            case MATRIX:
               c->type = MATRIX;
               c->data = addMatrix(a->data, b->data);
               break;
            case COMPLEX:
               c->type = MATRIX;
               c->data = addMatrixCmplx(a->data, b->data);
               break;
            case REAL:
               c->type = MATRIX;
               c->data = addMatrixReal(a->data, b->data);
               break;
            default:
               fprintf(stderr, "Error: Unknown number type\n");
               exit(0);
               break;
         }
         break;
      default:
         fprintf(stderr, "Error: Unknown number type\n");
         exit(0);
   }

   if(c->data == NULL){ free(c); c = NULL; }
   return c;
}

Number * subNumber(Number *a, Number *b){
   Number *c = newNumber();

   if(a == NULL || b == NULL) 
      { fprintf(stderr, "subNumber(NULL)\n"); exit(0); }

   switch(a->type){
      case REAL:
         switch(b->type){
            case REAL:
               c->type = REAL;
               c->data = subReal(a->data, b->data);
               break;
            case COMPLEX:
               c->type = COMPLEX;
               c->data = subRealCmplx(a->data, b->data);
               break;
            case MATRIX:
               c->type = MATRIX;
               c->data = subRealMatrix(a->data, b->data);
               break;
            default:
               fprintf(stderr, "Error: Unknown number type\n");
               exit(0);
               break;
         }
         break;
      case COMPLEX:
         switch(b->type){
            case COMPLEX:
               c->type = COMPLEX;
               c->data = subCmplx(a->data, b->data);
               break;
            case REAL:
               c->type = COMPLEX;
               c->data = subCmplxReal(a->data, b->data);
               break;
            case MATRIX:
               c->type = MATRIX;
               c->data = subCmplxMatrix(a->data, b->data);
               break;
            default:
               fprintf(stderr, "Error: Unknown number type\n");
               exit(0);
               break;
         }
         break;
      case MATRIX:
         switch(b->type){
            case MATRIX:
               c->type = MATRIX;
               c->data = subMatrix(a->data, b->data);
               break;
            case COMPLEX:
               c->type = MATRIX;
               c->data = subMatrixCmplx(a->data, b->data);
               break;
            case REAL:
               c->type = MATRIX;
               c->data = subMatrixReal(a->data, b->data);
               break;
            default:
               fprintf(stderr, "Error: Unknown number type\n");
               exit(0);
               break;
         }
         break;
      default:
         fprintf(stderr, "Error: Unknown number type\n");
         exit(0);
   }

   if(c->data == NULL){ free(c); c = NULL; }
   return c;
}
