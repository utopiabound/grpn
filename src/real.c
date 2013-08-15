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
/* real.c  by Paul Wilkins */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "real.h"
#include "mode.h"
#include "constant.h"
#include "float.h"

void checkFinite(Real *a){
   if(isnan(a->num)){
      a->ok = REAL_NAN;
      a->num = 0.0;
   } else if(!finite(a->num)){
      a->ok = REAL_INF;
      a->num = 0.0;
   }
}

/* convert a real number from the internal radix
   representation (radians) the the representation
   of the current mode (radians or degrees) */
Real * toRadixReal(Real *a){
   Real *r1;

   /* deal with degrees if we need to */
   if(getRadixMode() == DEGREES){
      r1 = mulReal(a, real180Pi);
   } else {
      r1 = setRealReal(newReal(), a);
   }
   checkFinite(r1);

   return r1;
}

Real * fromRadixReal(Real *a){
   Real *r1;

   /* deal with degrees if we need to */
   if(getRadixMode() == DEGREES){
      r1 = divReal(a, real180Pi);
   } else {
      r1 = setRealReal(newReal(), a);
   }
   checkFinite(r1);

   return r1;
}

/* create a new real number */
Real * newReal(){
   Real *p;

   if(NULL == (p = (Real *)malloc(sizeof(Real)))){
      perror("Malloc");
      exit(0);
   }
   p->ok = REAL_OK;
   p->num = 0.0;

   return p;
}

void freeReal(Real *a){
   free((char *)a);
}

double setDoubleReal(Real *a){
   if(a){
      switch(a->ok){
         case REAL_OK:
            return a->num; 
            break;
         case REAL_NAN:
            fprintf(stderr, "setDoubleReal trying to return NAN\n");
            return 0.0; 
            break;
         case REAL_INF:
            fprintf(stderr, "setDoubleReal trying to return INF\n");
            return 0.0; 
            break;
         default:
            fprintf(stderr, "Error: setDoubleReal: invalid type\n");
            exit(0);
      }
   }

   return 0.0;
}

Real * setRealDouble(Real *a, double d){
   a->ok = REAL_OK;
   a->num = d;
   checkFinite(a);

   return a;
}

Real * setRealReal(Real *a, Real *b){
   if(a && b){
      a->ok = b->ok;
      if(b->ok == REAL_OK){
         a->num = b->num;
         checkFinite(a);
      } else {
         a->num = 0.0;
      }
   }
   return a;
}

#define REAL_PRINT_SIZE 2048

char * printReal(Real *a){
   char *c, *p;
   int i;
   int baseMode;
   double dd, nn, fm;
   char buf[REAL_PRINT_SIZE];
   if(NULL == (c=(char *)malloc(REAL_PRINT_SIZE))){ perror("Malloc"); exit(0); }

   switch(a->ok){
      case REAL_OK:
         baseMode = getBaseMode();
         switch(baseMode){
            case BINARY:
               dd = a->num;
               i = 1;
               p = buf+REAL_PRINT_SIZE-1;
               *p-- = '\0';
               do {
                  nn = pow(2.0, (double)i);
                  if(fmod(dd, nn) >= nn*0.5) *p-- = '1';
                  else *p-- = '0';
                  i++;
               } while (dd / nn >= 0.5);
               *p = '\0';
 
               if(i <= 2){
                  strcpy(c, "0");
               } else {
		  strncpy(c, p+2, i-1);
               }
               break;
            case OCTAL:
               dd = a->num;
               i = 1;
               p = buf+REAL_PRINT_SIZE-1;
               *p-- = '\0';
               do {
                  nn = pow(8.0, (double)i);
                  fm = fmod(dd, nn);
                  if(fm >= nn*0.875) *p-- = '7';
                  else if(fm >= nn*0.75) *p-- = '6';
                  else if(fm >= nn*0.625) *p-- = '5';
                  else if(fm >= nn*0.5) *p-- = '4';
                  else if(fm >= nn*0.375) *p-- = '3';
                  else if(fm >= nn*0.25) *p-- = '2';
                  else if(fm >= nn*0.125) *p-- = '1';
                  else *p-- = '0';
                  i++;
               } while (dd / nn >= 0.125);
 
               if(i <= 2){
                  strcpy(c, "00");
               } else {
		  strncpy(c, p+1, i-0);
               }
               break;
            case DECIMAL:
               sprintf(c, "%.*g",PRINT_REAL_DIG, a->num);
               break;
            case DECIMAL_ENG:
                {
         	   double mantissa;
                   int exponent;
                   char * pch;
          
                   sprintf(c, "%.*e",DBL_DIG, a->num);
                   
                   pch = strtok (c,"eE");
                   if (pch != NULL) {
                      sscanf(pch,"%lg",&mantissa);
                   } else {
                      fprintf(stderr, "Error: printReal: no mumber\n");
                   };
          
                   pch = strtok (NULL,"eE");
                   if (pch != NULL) {   
                      sscanf(pch,"%d",&exponent);
                   } else {
                      fprintf(stderr, "Error: printReal: no exponent\n");
                   };
                   // matter of taste, alternative : >5 || <-2 ?
                   if ( (exponent>2)||(exponent<-0) ) { 
                      switch (exponent%3) { 
                        case 2:  
                           mantissa*=10; 
                           exponent--; 
                        case 1: 
                           mantissa*=10; 
                           exponent--; 
                        break; 
                        case -1: 
                           mantissa*=10; 
                           exponent--; 
                        case -2: 
                           mantissa*=10; 
                           exponent--; 
                        break; 
                     }      
                     sprintf(c,"%.*ge%+d",PRINT_REAL_DIG,mantissa,exponent); 
                   } else  { 
                     sprintf(c,"%.*g",PRINT_REAL_DIG,a->num); 
                   };
                }
                break;
            case HEXADECIMAL:
               dd = a->num;
               i = 1;
               p = buf+REAL_PRINT_SIZE-1;
               *p-- = '\0';
               do {
                  nn = pow(16.0, (double)i);
                  fm = fmod(dd, nn);
                  if(fm >= nn*0.9375) *p-- = 'f';
                  else if(fm >= nn*0.875) *p-- = 'e';
                  else if(fm >= nn*0.8125) *p-- = 'd';
                  else if(fm >= nn*0.75) *p-- = 'c';
                  else if(fm >= nn*0.6875) *p-- = 'b';
                  else if(fm >= nn*0.625) *p-- = 'a';
                  else if(fm >= nn*0.5625) *p-- = '9';
                  else if(fm >= nn*0.5) *p-- = '8';
                  else if(fm >= nn*0.4375) *p-- = '7';
                  else if(fm >= nn*0.375) *p-- = '6';
                  else if(fm >= nn*0.3125) *p-- = '5';
                  else if(fm >= nn*0.25) *p-- = '4';
                  else if(fm >= nn*0.1875) *p-- = '3';
                  else if(fm >= nn*0.125) *p-- = '2';
                  else if(fm >= nn*0.0625) *p-- = '1';
                  else *p-- = '0';
                  i++;
               } while (dd / nn >= 0.0625);
 
               if(i <= 2){
                  strcpy(c, "0x0");
               } else {
		  strcpy(c, "0x");
		  strncat(c, p+2, i-1);
               }
               break;
         }
         break;
      case REAL_NAN:
         sprintf(c, "NaN");
         break;
      case REAL_INF:
         sprintf(c, "Infinity");
         break;
      default:
         fprintf(stderr, "Error: printReal: invalid type\n");
         exit(0);
   }
   return c;
}

/* compare 2 Real numbers */
int cmpReal(Real *a, Real *b){
   if(a->ok == REAL_OK && b->ok == REAL_OK){
      if(a->num == b->num) return 0;
      if(a->num > b->num) return 1;
      else return -1;
   }
   else if(a->ok == REAL_INF && b->ok == REAL_INF)
      return 0;
   else 
      return 1;
}

/* is the Real numbers an int ? */
int isIntReal(Real *a){
   if(a->ok == REAL_OK){
      if(1.0 == (double)( ((int)(a->num)) /a->num) ) return 1;
   }
   return 0;
}

/* absolute value of a Real number */
Real * absReal(Real *a){
   Real *p = newReal();
   p->ok = a->ok;
   p->num = fabs(a->num);
   checkFinite(p);
   return p;
}

/* negate a Real number */
Real * negReal(Real *a){
   Real *p = newReal();
   p->ok = a->ok;
   p->num = -(a->num);
   checkFinite(p);
   return p;
}

/* negate a Real number */
Real * negEqReal(Real *a){
   a->num = -a->num;
   checkFinite(a);
   return a;
}

/* invert a Real number */
Real * invReal(Real *a){
   Real *p = newReal();
   p->ok = a->ok;
   if(a->num == 0.0){
      p->ok = REAL_NAN;
      p->num = 0.0;
   } else {
      p->num = 1.0 / a->num;
      checkFinite(p);
   }

   return p;
}

/* invert a Real number */
Real * invEqReal(Real *a){
   if(a->num == 0.0){
      a->ok = REAL_NAN;
      a->num = 0.0;
   } else {
      a->num = 1.0 / a->num;
      checkFinite(a);
   }
   return a;
}

/* Real number to the power */
Real * powReal(Real *a, Real *b){
   Real *p = newReal();

   if(a->ok == REAL_OK && b->ok == REAL_OK){
      p->ok = REAL_OK;
      p->num = pow(a->num, b->num);
      checkFinite(p);

   } else if(a->ok == REAL_NAN || b->ok == REAL_NAN){
      p->ok = REAL_NAN;
      p->num = 0.0;

   } else {
      p->ok = REAL_INF;
      p->num = 0.0;
   }

   return p;
}

/* Real number to the power */
Real * powEqReal(Real *a, Real *b){
   if(a->ok == REAL_OK && b->ok == REAL_OK){
      a->num = pow(a->num, b->num);
      checkFinite(a);

   } else if(a->ok == REAL_NAN || b->ok == REAL_NAN){
      a->num = 0.0;

   }

   return a;
}


/* Real number to the power */
Real * powRealInt(Real *a, int b){
   Real *p = newReal();
   p->ok = a->ok;
   p->num = pow(a->num, (double)b);
   checkFinite(p);

   return p;
}

/* natural log of as Real number */
Real * lnReal(Real *a){
   Real *p = newReal();
      
   switch(a->ok){
      case REAL_OK:
         if(a->num > 0.0){
            p->ok = a->ok;
            p->num = log(a->num);
	    checkFinite(p);
         }
         else if(a->num == 0.0){
            p->ok = REAL_INF;
            p->num = 0.0;
         } else {
            printf("lnReal(<0)\n");
            exit(0);
         }
         break;
      case REAL_INF:
         p->ok = REAL_INF;
         p->num = 0.0;
         break;
      case REAL_NAN:
         p->ok = REAL_NAN;
         p->num = 0.0;
         break;
      default:
         fprintf(stderr, "lnReal unknown real type\n");
         exit(0);
         break;
   }
         
   return p;
}

/* natural log of as Real number */
Real * lnEqReal(Real *a){
   switch(a->ok){
      case REAL_OK:
         if(a->num > 0.0){
            a->num = log(a->num);
	    checkFinite(a);
         }
         else if(a->num == 0.0){
            a->ok = REAL_INF;
            a->num = 0.0;
         } else {
            printf("lnEqReal(<0)\n");
            exit(0);
         }
         break;
      case REAL_INF:
         a->num = 0.0;
         break;
      case REAL_NAN:
         a->num = 0.0;
         break;
      default:
         fprintf(stderr, "lnReal unknown real type\n");
         exit(0);
         break;
   }
         
   return a;
}

/* log base ten of a Real number */
Real * logReal(Real *a){
   Real *p = newReal();
      
   switch(a->ok){
      case REAL_OK:
         if(a->num > 0.0){
            p->ok = a->ok;
            p->num = log10(a->num);
	    checkFinite(p);
         }
         else if(a->num == 0.0){
            p->ok = REAL_INF;
            p->num = 0.0;
         } else {
            printf("logReal(<0)\n");
            exit(0);
         }
         break;
      case REAL_INF:
         p->ok = REAL_INF;
         p->num = 0.0;
         break;
      case REAL_NAN:
         p->ok = REAL_NAN;
         p->num = 0.0;
         break;
      default:
         fprintf(stderr, "logReal unknown real type\n");
         exit(0);
         break;
   }
         
   return p;
}

/* log base ten of a Real number */
Real * logEqReal(Real *a){
   switch(a->ok){
      case REAL_OK:
         if(a->num > 0.0){
            a->num = log10(a->num);
	    checkFinite(a);
         }
         else if(a->num == 0.0){
            a->ok = REAL_INF;
            a->num = 0.0;
         } else {
            printf("logEqReal(<0)\n");
            exit(0);
         }
         break;
      case REAL_INF:
         a->num = 0.0;
         break;
      case REAL_NAN:
         a->num = 0.0;
         break;
      default:
         fprintf(stderr, "logEqReal unknown real type\n");
         exit(0);
         break;
   }
   return a;
}

/* e to a Real number */
Real * expReal(Real *a){
   Real *p = newReal();
   p->ok = a->ok;
   p->num = exp(a->num);
   checkFinite(p);
   return p;
}

/* e to a Real number */
Real * expEqReal(Real *a){
   a->num = exp(a->num);
   checkFinite(a);
   return a;
}


/* trig func */
Real * asinReal(Real *a){
   Real *p = newReal();
   p->ok = a->ok;
   p->num = asin(a->num);
   checkFinite(p);
   return p;
}

/* trig func */
Real * asinEqReal(Real *a){
   a->num = asin(a->num);
   checkFinite(a);
   return a;
}

/* trig func */
Real * acosReal(Real *a){
   Real *p = newReal();
   p->ok = a->ok;
   p->num = acos(a->num);
   checkFinite(p);
   return p;
}

/* trig func */
Real * acosEqReal(Real *a){
   a->num = acos(a->num);
   checkFinite(a);
   return a;
}

/* trig func */
Real * atanReal(Real *a){
   Real *p = newReal();
   p->ok = a->ok;
   p->num = atan(a->num);
   checkFinite(p);
   return p;
}

/* trig func */
Real * atan2Real(Real *a, Real *b){
   Real *p = newReal();
   if(a->ok == REAL_OK && b->ok == REAL_OK){
      p->ok = REAL_OK;
      p->num = atan2(a->num, b->num);
      checkFinite(p);
   } else if(a->ok == REAL_NAN || b->ok == REAL_NAN){
      p->ok = REAL_NAN;
      p->num = 0.0;
   } else {
      p->ok = REAL_INF;
      p->num = 0.0;
   }
   return p;
}

/* trig func */
Real * atanEqReal(Real *a){
   a->num = atan(a->num);
   checkFinite(a);
   return a;
}

/* trig func */
Real * sinReal(Real *a){
   Real *p = newReal();
   p->ok = a->ok;
   p->num = sin(a->num);
   checkFinite(p);
   return p;
}

/* trig func */
Real * sinEqReal(Real *a){
   a->num = sin(a->num);
   checkFinite(a);
   return a;
}

/* trig func */
Real * cosReal(Real *a){
   Real *p = newReal();

   p->ok = a->ok;
   p->num = cos(a->num);
   checkFinite(p);
   return p;
}

/* trig func */
Real * cosEqReal(Real *a){
   a->num = cos(a->num);
   checkFinite(a);
   return a;
}

/* trig func */
Real * tanReal(Real *a){
   Real *p = newReal();
   p->ok = a->ok;
   p->num = tan(a->num);
   checkFinite(p);
   return p;
}

/* trig func */
Real * tanEqReal(Real *a){
   a->num = tan(a->num);
   checkFinite(a);
   return a;
}

/* multiply 2 Real numbers */
Real * mulReal(Real *a, Real *b){
   Real *p = newReal();

   if(a->ok == REAL_OK && b->ok == REAL_OK){
      p->ok = REAL_OK;
      p->num = a->num * b->num;
      checkFinite(p);
   }
   else if(a->ok == REAL_NAN || b->ok == REAL_NAN) p->ok = REAL_NAN;
   else p->ok = REAL_INF;

   return p;
}

/* multiply 2 Real numbers */
Real * mulEqReal(Real *a, Real *b){
   if(a->ok == REAL_OK && b->ok == REAL_OK){
      a->num *= b->num;
      checkFinite(a);
   }
   else if(a->ok == REAL_NAN || b->ok == REAL_NAN) a->ok = REAL_NAN;
   else a->ok = REAL_INF;

   return a;
}


   
/* divide 2 Real numbers */
Real * divReal(Real *a, Real *b){
   Real *p = newReal();

   switch(a->ok){
      case REAL_OK:
         switch(b->ok){
            case REAL_OK:
               if(b->num == 0.0){
                  p->ok = REAL_NAN;
                  p->num = 0.0;
               } else {
                  p->ok = REAL_OK;
                  p->num = a->num / b->num;
		  checkFinite(p);
               }
               break;
            case REAL_INF:
               p->ok = REAL_NAN;
               p->num = 0.0;
               break;
            case REAL_NAN:
               p->ok = REAL_NAN;
               p->num = 0.0;
               break;
            default:
               fprintf(stderr, "divReal unknown real type\n");
               exit(0);
               break;
         }
         break;
      case REAL_INF:
         p->ok = REAL_INF;
         p->num = 0.0;
         break;
      case REAL_NAN:
         p->ok = REAL_NAN;
         p->num = 0.0;
         break;
      default:
         fprintf(stderr, "divReal unknown real type\n");
         exit(0);
         break;
   }

   return p;
}


/* modulo 2 Real numbers */
Real * modReal(Real *a, Real *b){
   Real *p = newReal();

   switch(a->ok){
      case REAL_OK:
         switch(b->ok){
            case REAL_OK:
               if(b->num == 0.0){
                  p->ok = REAL_NAN;
                  p->num = 0.0;
               } else {
                  p->ok = REAL_OK;
                  p->num = fmod(a->num , b->num);
		  checkFinite(p);
               }
               break;
            case REAL_INF:
               p->ok = REAL_NAN;
               p->num = 0.0;
               break;
            case REAL_NAN:
               p->ok = REAL_NAN;
               p->num = 0.0;
               break;
            default:
               fprintf(stderr, "modReal unknown real type\n");
               exit(0);
               break;
         }
         break;
      case REAL_INF:
         p->ok = REAL_INF;
         p->num = 0.0;
         break;
      case REAL_NAN:
         p->ok = REAL_NAN;
         p->num = 0.0;
         break;
      default:
         fprintf(stderr, "modReal unknown real type\n");
         exit(0);
         break;
   }

   return p;
}


/* divide 2 Real numbers */
Real * divEqReal(Real *a, Real *b){

   switch(a->ok){
      case REAL_OK:
         switch(b->ok){
            case REAL_OK:
               if(b->num == 0.0){
                  a->ok = REAL_NAN;
                  a->num = 0.0;
               } else {
                  a->num /= b->num;
		  checkFinite(a);
               }
               break;
            case REAL_INF:
               a->ok = REAL_NAN;
               a->num = 0.0;
               break;
            case REAL_NAN:
               a->ok = REAL_NAN;
               a->num = 0.0;
               break;
            default:
               fprintf(stderr, "divReal unknown real type\n");
               exit(0);
               break;
         }
         break;
   }

   return a;
}

/* add 2 Real numbers */
Real * addReal(Real *a, Real *b){
   Real *p = newReal();

   if(a->ok == REAL_OK && b->ok == REAL_OK){
      p->ok = REAL_OK;
      p->num = a->num + b->num;
      checkFinite(p);
   }
   else if(a->ok == REAL_NAN || b->ok == REAL_NAN) p->ok = REAL_NAN;
   else p->ok = REAL_INF;

   return p;
}

/* add 2 Real numbers */
Real * addEqReal(Real *a, Real *b){

   if(a->ok == REAL_OK && b->ok == REAL_OK){
      a->num += b->num;
      checkFinite(a);
   }
   else if(a->ok == REAL_NAN || b->ok == REAL_NAN) a->ok = REAL_NAN;
   else a->ok = REAL_INF;

   return a;
}


/* subtract 2 Real numbers */
Real * subReal(Real *a, Real *b){
   Real *p = newReal();

   if(a->ok == REAL_OK && b->ok == REAL_OK){
      p->ok = REAL_OK;
      p->num = a->num - b->num;
      checkFinite(p);
   }
   else if(a->ok == REAL_NAN || b->ok == REAL_NAN) p->ok = REAL_NAN;
   else p->ok = REAL_INF;

   return p;
}

/* subtract 2 Real numbers */
Real * subEqReal(Real *a, Real *b){

   if(a->ok == REAL_OK && b->ok == REAL_OK){
      a->num -= b->num;
      checkFinite(a);
   }
   else if(a->ok == REAL_NAN || b->ok == REAL_NAN) a->ok = REAL_NAN;
   else a->ok = REAL_INF;

   return a;
}

/* left shift 2 Real Numbers */
Real * lshiftReal(Real *a, Real *b){
	 Real *p = newReal();

	 if(a->ok == REAL_OK && b->ok == REAL_OK){
		  p->ok = REAL_OK;
		  p->num = (ulong)a->num << (ulong)b->num;
		  checkFinite(p);
	 }
	 else if(a->ok == REAL_NAN || b->ok == REAL_NAN) p->ok = REAL_NAN;
	 else p->ok = REAL_INF;

	 return p;
}

/* right shift 2 Real Numbers */
Real * rshiftReal(Real *a, Real *b){
	 Real *p = newReal();

	 if(a->ok == REAL_OK && b->ok == REAL_OK){
		  p->ok = REAL_OK;
		  p->num = (ulong)a->num >> (ulong)b->num;
		  checkFinite(p);
	 }
	 else if(a->ok == REAL_NAN || b->ok == REAL_NAN) p->ok = REAL_NAN;
	 else p->ok = REAL_INF;

	 return p;
}
