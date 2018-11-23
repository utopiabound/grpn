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
/* matrix.c  by Paul Wilkins */

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <string.h>

#include "matrix.h"
#include "number.h"
#include "error.h"
#include "constant.h"

/* get the space for the data */
Number ** mallocData(Number **d, int rows, int cols){
   int size;
   Number **p;

   if(d){
      size = rows*cols*sizeof(Number *);
      if(NULL == (p=(Number **)realloc((char *)d, size))){
         perror("realloc"); exit(0);
      }
   } else {
      if(NULL == (p = (Number **)malloc(rows*cols*sizeof(Number *)))){
         perror("Malloc"); exit(0);
      }
   }
   return p;
}


/* create a new matrix */
Matrix * newMatrix(){
   Matrix *p;
   if(NULL == (p = (Matrix *)malloc(sizeof(Matrix)))){
      perror("Malloc");
      exit(0);
   }
   p->rows = 0;
   p->cols = 0;
   p->data = NULL;

   return p;
}

void freeMatrix(Matrix *a){
   int i, j;
   Number **ptr;
   if(a->data){
      for(i=0; i<a->rows; i++)
         for(j=0; j<a->cols; j++){
            ptr = a->data + (a->cols * i) + j;
            freeNumber(*ptr);
         }
      free((char *)a->data);
   }
   free((char *)a);
}

void setMatrix(Matrix *a, Number *b, int row, int col){
   int oldrows, oldcols;
   Number **ptr;
   row++; col++;
   if(a->data == NULL){
      if(row > a->rows) a->rows = row;
      if(col > a->cols) a->cols = col;
      a->data = mallocData(NULL, a->rows, a->cols);
      memset(a->data, 0, a->rows*a->cols*sizeof(Number *));
   } 
   else if(row > a->rows || col > a->cols){
      oldrows = a->rows; oldcols = a->cols;
      if(row > a->rows) a->rows = row;
      if(col > a->cols) a->cols = col;
      a->data = mallocData(a->data, a->rows, a->cols);
      memset(a->data+(oldrows*oldcols), 0, 
         ((a->rows*a->cols)-(oldrows*oldcols))*sizeof(Number *));
   }

   row--; col--;
   ptr = a->data + (a->cols * row) + col;
   *ptr = setNumberNumber(newNumber(), b);
}

Matrix * setMatrixMatrix(Matrix *a, Matrix *b){
   int i, j;
   Number **ptr1, **ptr2;

   if(a == NULL || b == NULL) {
       fprintf(stderr, "setMatrixMatrix(NULL)\n"); exit(0);
   }

   /* initalize the stuff in p */
   a->data = mallocData(NULL, b->rows, b->cols);
   a->rows = b->rows;
   a->cols = b->cols;

   /* p[i][j] = b + a[i][j] */
   for(i=0; i<a->rows; i++)
      for(j=0; j<a->cols; j++){
         ptr1 = b->data + (a->cols * i) + j;
         ptr2 = a->data + (a->cols * i) + j;
         *ptr2 = setNumberNumber(newNumber(), *ptr1);
      }

   return a;
}


#define MATRIX_PRINT_SIZE 50

char * printMatrixShort(Matrix *a){
   char *c;
   if(NULL == (c=(char *)malloc(MATRIX_PRINT_SIZE)))
      { perror("Malloc"); exit(0); }

   sprintf(c, "[%d x %d Matrix]", a->rows, a->cols);
   return c;
}

char * printMatrix(Matrix *a){
   char *c;
   char *p1;
   int i, j;

   if(NULL == (c=(char *)malloc(MATRIX_PRINT_SIZE*a->rows*a->cols)))
      { perror("Malloc"); exit(0); }

   *c = '\0';
   if(a->data){
      strcat(c, "[ ");
      for(i=0; i<a->rows; i++){
         for(j=0; j<a->cols; j++){
            strcat(c, (p1=printNumberShort(*(a->data+(a->cols*i)+j))));
            strcat(c, " ");
            free(p1);
         }
         if(i < a->rows-1) strcat(c, " \n");
      }
      strcat(c, "]");
   }

   return c;
}

Matrix * negMatrix(Matrix *a){
   int i, j;
   Number **ptr1, **ptr2;
   Matrix *p = newMatrix();

   if(a == NULL) { fprintf(stderr, "negMatrix(NULL)\n"); exit(0); }

   /* initalize the stuff in p */
   p->data = mallocData(NULL, a->rows, a->cols);
   p->rows = a->rows;
   p->cols = a->cols;

   /* p[i][j] = b + a[i][j] */
   for(i=0; i<a->rows; i++)
      for(j=0; j<a->cols; j++){
         ptr1 = a->data + (a->cols * i) + j;
         ptr2 = p->data + (a->cols * i) + j;
         *ptr2 = negNumber(*ptr1);
      }

   return p;
}

Matrix * invMatrix(Matrix *a){
   printf("TODO: Inv Matrix\n");

   return NULL;
}

/***************** MULTIPLY *******************/

/* multiply 2 Matrixes  */
Matrix * mulMatrix(Matrix *a, Matrix *b){
   int i, j, k;
   Matrix *p;
   Number *n1, *n2, *nsum;
   Number **ptr1, **ptr2, **ptr3;

   if(a == NULL || b == NULL) 
      { fprintf(stderr, "mulMatrix(NULL)\n"); exit(0); }

   if(a->cols != b->rows){
      sprintf(getStringError(), "* Error: Invalid dimension.");
      return NULL;
   }

   /* initalize the stuff in p */
   p = newMatrix();
   p->data = mallocData(NULL, a->rows, b->cols);
   p->rows = a->rows;
   p->cols = b->cols;

   /* p[i][j] = sum t=1 to k  of a[i][t]*b[t][j] */
   for(i=0; i<a->rows; i++){
      for(j=0; j<b->cols; j++){
         ptr3 = p->data + (p->cols * i) + j;
         nsum = setNumberReal(newNumber(), realZero);
         for(k=0; k<a->cols; k++){
            ptr1 = a->data + (a->cols * i) + k;
            ptr2 = b->data + (b->cols * k) + j;
            n1 = mulNumber(*ptr1, *ptr2);
            n2 = nsum;
            nsum = addNumber(n2, n1);
            freeNumber(n1);
            freeNumber(n2);
            
         }
         *ptr3 = nsum;
      }
   }

   return p;
}

   
/* multiply a Matrix and a Cmplx */
Matrix * mulMatrixCmplx(Matrix *a, Cmplx *b){
   int i, j;
   Number *n1;
   Number **ptr1, **ptr2;
   Matrix *p = newMatrix();

   if(a == NULL) { fprintf(stderr, "mulMatrixCmplx(NULL)\n"); exit(0); }

   /* initalize the stuff in p */
   p->data = mallocData(NULL, a->rows, a->cols);
   p->rows = a->rows;
   p->cols = a->cols;

   /* p[i][j] = b + a[i][j] */
   for(i=0; i<a->rows; i++)
      for(j=0; j<a->cols; j++){
         ptr1 = a->data + (a->cols * i) + j;
         ptr2 = p->data + (a->cols * i) + j;
         *ptr2 = mulNumber(*ptr1, (n1=setNumberCmplx(newNumber(), b)));
         freeNumber(n1);
      }

   return p;
}

/* multiply a Matrix and a Real */
Matrix * mulMatrixReal(Matrix *a, Real *b){
   int i, j;
   Number *n1;
   Number **ptr1, **ptr2;
   Matrix *p = newMatrix();

   if(a == NULL) { fprintf(stderr, "mulMatrixReal(NULL)\n"); exit(0); }

   /* initalize the stuff in p */
   p->data = mallocData(NULL, a->rows, a->cols);
   p->rows = a->rows;
   p->cols = a->cols;

   /* p[i][j] = b + a[i][j] */
   for(i=0; i<a->rows; i++)
      for(j=0; j<a->cols; j++){
         ptr1 = a->data + (a->cols * i) + j;
         ptr2 = p->data + (a->cols * i) + j;
         *ptr2 = mulNumber(*ptr1, (n1=setNumberReal(newNumber(), b)));
         freeNumber(n1);
      }

   return p;
}


/***************** DIVIDE *******************/

/* divide 2 Matrixes  */
Matrix * divMatrix(Matrix *a, Matrix *b){
   invalidTyprError("/");
   return NULL;
}

/* divide a Cmplx by a Matrix */
Matrix * divCmplxMatrix(Cmplx *a, Matrix *b){
   invalidTyprError("/");
   return NULL;
}

/* divide a Cmplx by a Matrix */
Matrix * divRealMatrix(Real *a, Matrix *b){
   invalidTyprError("/");
   return NULL;
}

/* divide a Matrix by a Cmplx */
Matrix * divMatrixCmplx(Matrix *a, Cmplx *b){
   int i, j;
   Number *n1;
   Number **ptr1, **ptr2;
   Matrix *p = newMatrix();

   if(a == NULL) { fprintf(stderr, "divMatrixCmplx(NULL)\n"); exit(0); }

   /* initalize the stuff in p */
   p->data = mallocData(NULL, a->rows, a->cols);
   p->rows = a->rows;
   p->cols = a->cols;

   /* p[i][j] = b + a[i][j] */
   for(i=0; i<a->rows; i++)
      for(j=0; j<a->cols; j++){
         ptr1 = a->data + (a->cols * i) + j;
         ptr2 = p->data + (a->cols * i) + j;
         *ptr2 = divNumber(*ptr1, (n1=setNumberCmplx(newNumber(), b)));
         freeNumber(n1);
      }

   return p;
}

/* divide a Matrix by a Real */
Matrix * divMatrixReal(Matrix *a, Real *b){
   int i, j;
   Number *n1;
   Number **ptr1, **ptr2;
   Matrix *p = newMatrix();

   if(a == NULL) { fprintf(stderr, "divMatrixReal(NULL)\n"); exit(0); }

   /* initalize the stuff in p */
   p->data = mallocData(NULL, a->rows, a->cols);
   p->rows = a->rows;
   p->cols = a->cols;

   /* p[i][j] = b + a[i][j] */
   for(i=0; i<a->rows; i++)
      for(j=0; j<a->cols; j++){
         ptr1 = a->data + (a->cols * i) + j;
         ptr2 = p->data + (a->cols * i) + j;
         *ptr2 = divNumber(*ptr1, (n1=setNumberReal(newNumber(), b)));
         freeNumber(n1);
      }

   return p;
}

/***************** MODULO *******************/

/* modulo 2 Matrixes  */
Matrix * modMatrix(Matrix *a, Matrix *b){
   invalidTyprError("mod");
   return NULL;
}

/* modulo a Cmplx by a Matrix */
Matrix * modCmplxMatrix(Cmplx *a, Matrix *b){
   invalidTyprError("mod");
   return NULL;
}

/* modulo a Cmplx by a Matrix */
Matrix * modRealMatrix(Real *a, Matrix *b){
   invalidTyprError("mod");
   return NULL;
}

/* modulo a Matrix by a Cmplx */
Matrix * modMatrixCmplx(Matrix *a, Cmplx *b){
   invalidTyprError("mod");
   return NULL;
}


/* modulo a Matrix by a Real */
Matrix * modMatrixReal(Matrix *a, Real *b){
   int i, j;
   Number *n1;
   Number **ptr1, **ptr2;
   Matrix *p = newMatrix();

   if(a == NULL) { fprintf(stderr, "modMatrixReal(NULL)\n"); exit(0); }

   /* initalize the stuff in p */
   p->data = mallocData(NULL, a->rows, a->cols);
   p->rows = a->rows;
   p->cols = a->cols;

   /* p[i][j] = b + a[i][j] */
   for(i=0; i<a->rows; i++)
      for(j=0; j<a->cols; j++){
         ptr1 = a->data + (a->cols * i) + j;
         ptr2 = p->data + (a->cols * i) + j;
         *ptr2 = modNumber(*ptr1, (n1=setNumberReal(newNumber(), b)));
         freeNumber(n1);
      }

   return p;
}




/***************** ADD *******************/

/* add 2 Matrixes  */
Matrix * addMatrix(Matrix *a, Matrix *b){
   int i, j;
   Number **ptr1, **ptr2, **ptr3;
   Matrix *p;

   if(a == NULL || b == NULL) 
      { fprintf(stderr, "addMatrix(NULL)\n"); exit(0); }

   if(a->rows != b->rows || a->cols != b->cols){
      sprintf(getStringError(), "+ Error: Invalid dimension.");
      return NULL;
   }

   /* initalize the stuff in p */
   p = newMatrix();
   p->data = mallocData(NULL, a->rows, a->cols);
   p->rows = a->rows;
   p->cols = a->cols;

   /* p[i][j] = b + a[i][j] */
   for(i=0; i<a->rows; i++)
      for(j=0; j<a->cols; j++){
         ptr1 = a->data + (a->cols * i) + j;
         ptr2 = b->data + (a->cols * i) + j;
         ptr3 = p->data + (a->cols * i) + j;
         *ptr3 = addNumber(*ptr1, *ptr2);
      }

   return p;
}

/* add a Matrix and a Cmplx */
Matrix * addMatrixCmplx(Matrix *a, Cmplx *b){
   int i, j;
   Number *n1;
   Number **ptr1, **ptr2;
   Matrix *p = newMatrix();

   if(a == NULL) { fprintf(stderr, "addMatrixCmplx(NULL)\n"); exit(0); }

   /* initalize the stuff in p */
   p->data = mallocData(NULL, a->rows, a->cols);
   p->rows = a->rows;
   p->cols = a->cols;

   /* p[i][j] = b + a[i][j] */
   for(i=0; i<a->rows; i++)
      for(j=0; j<a->cols; j++){
         ptr1 = a->data + (a->cols * i) + j;
         ptr2 = p->data + (a->cols * i) + j;
         *ptr2 = addNumber(*ptr1, (n1=setNumberCmplx(newNumber(), b)));
         freeNumber(n1);
      }

   return p;
}

/* add a Matrix and a Real */
Matrix * addMatrixReal(Matrix *a, Real *b){
   int i, j;
   Number *n1;
   Number **ptr1, **ptr2;
   Matrix *p = newMatrix();

   if(a == NULL) { fprintf(stderr, "addMatrixReal(NULL)\n"); exit(0); }

   /* initalize the stuff in p */
   p->data = mallocData(NULL, a->rows, a->cols);
   p->rows = a->rows;
   p->cols = a->cols;

   /* p[i][j] = b + a[i][j] */
   for(i=0; i<a->rows; i++)
      for(j=0; j<a->cols; j++){
         ptr1 = a->data + (a->cols * i) + j;
         ptr2 = p->data + (a->cols * i) + j;
         *ptr2 = addNumber(*ptr1, (n1=setNumberReal(newNumber(), b)));
         freeNumber(n1);
      }

   return p;
}



/***************** SUBTRACT *******************/

/* subtract 2 Matrixes  */
Matrix * subMatrix(Matrix *a, Matrix *b){
   int i, j;
   Number **ptr1, **ptr2, **ptr3;
   Matrix *p;

   if(a == NULL || b == NULL) 
      { fprintf(stderr, "subMatrix(NULL)\n"); exit(0); }

   if(a->rows != b->rows || a->cols != b->cols){
      sprintf(getStringError(), "- Error: Invalid dimension.");
      return NULL;
   }

   /* initalize the stuff in p */
   p = newMatrix();
   p->data = mallocData(NULL, a->rows, a->cols);
   p->rows = a->rows;
   p->cols = a->cols;

   /* p[i][j] = b + a[i][j] */
   for(i=0; i<a->rows; i++)
      for(j=0; j<a->cols; j++){
         ptr1 = a->data + (a->cols * i) + j;
         ptr2 = b->data + (a->cols * i) + j;
         ptr3 = p->data + (a->cols * i) + j;
         *ptr3 = subNumber(*ptr1, *ptr2);
      }

   return p;
}

/* subtract a Cmplx from a Matrix */
Matrix * subCmplxMatrix(Cmplx *a, Matrix *b){
   invalidTyprError("-");
   return NULL;
}

/* subtract a Real from a Matrix */
Matrix * subRealMatrix(Real *a, Matrix *b){
   invalidTyprError("-");
   return NULL;
}

/* subtract a Cmplx from a Matrix */
Matrix * subMatrixCmplx(Matrix *a, Cmplx *b){
   int i, j;
   Number *n1;
   Number **ptr1, **ptr2;
   Matrix *p = newMatrix();

   if(a == NULL) { fprintf(stderr, "subMatrixCmplx(NULL)\n"); exit(0); }

   /* initalize the stuff in p */
   p->data = mallocData(NULL, a->rows, a->cols);
   p->rows = a->rows;
   p->cols = a->cols;

   /* p[i][j] = b + a[i][j] */
   for(i=0; i<a->rows; i++)
      for(j=0; j<a->cols; j++){
         ptr1 = a->data + (a->cols * i) + j;
         ptr2 = p->data + (a->cols * i) + j;
         *ptr2 = subNumber(*ptr1, (n1=setNumberCmplx(newNumber(), b)));
         freeNumber(n1);
      }

   return p;
}

/* subtract a Real from a Matrix */
Matrix * subMatrixReal(Matrix *a, Real *b){
   int i, j;
   Number *n1;
   Number **ptr1, **ptr2;
   Matrix *p = newMatrix();

   if(a == NULL) { fprintf(stderr, "subMatrixReal(NULL)\n"); exit(0); }

   /* initalize the stuff in p */
   p->data = mallocData(NULL, a->rows, a->cols);
   p->rows = a->rows;
   p->cols = a->cols;

   /* p[i][j] = b + a[i][j] */
   for(i=0; i<a->rows; i++)
      for(j=0; j<a->cols; j++){
         ptr1 = a->data + (a->cols * i) + j;
         ptr2 = p->data + (a->cols * i) + j;
         *ptr2 = subNumber(*ptr1, (n1=setNumberReal(newNumber(), b)));
         freeNumber(n1);
      }

   return p;
}

/* left shift a matrix */
Matrix * lshiftMatrixReal(Matrix *a, Real *b){
   int i, j;
   Number *n1;
   Number **ptr1, **ptr2;
   Matrix *p = newMatrix();

   if(a == NULL) { fprintf(stderr, "subMatrixReal(NULL)\n"); exit(0); }

   /* initalize the stuff in p */
   p->data = mallocData(NULL, a->rows, a->cols);
   p->rows = a->rows;
   p->cols = a->cols;

   /* p[i][j] = b + a[i][j] */
   for(i=0; i<a->rows; i++)
      for(j=0; j<a->cols; j++){
         ptr1 = a->data + (a->cols * i) + j;
         ptr2 = p->data + (a->cols * i) + j;
         *ptr2 = lShiftNumber(*ptr1, (n1=setNumberReal(newNumber(), b)));
         freeNumber(n1);
      }

   return p;
}

/* right shift a matrix */
Matrix * rshiftMatrixReal(Matrix *a, Real *b){
   int i, j;
   Number *n1;
   Number **ptr1, **ptr2;
   Matrix *p = newMatrix();

   if(a == NULL) { fprintf(stderr, "subMatrixReal(NULL)\n"); exit(0); }

   /* initalize the stuff in p */
   p->data = mallocData(NULL, a->rows, a->cols);
   p->rows = a->rows;
   p->cols = a->cols;

   /* p[i][j] = b + a[i][j] */
   for(i=0; i<a->rows; i++)
      for(j=0; j<a->cols; j++){
         ptr1 = a->data + (a->cols * i) + j;
         ptr2 = p->data + (a->cols * i) + j;
         *ptr2 = rShiftNumber(*ptr1, (n1=setNumberReal(newNumber(), b)));
         freeNumber(n1);
      }

   return p;
}
