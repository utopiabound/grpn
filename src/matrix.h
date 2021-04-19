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
/* matrix.h  by Paul Wilkins */
/* Here we set up the matrix numbers */

#ifndef __MATRIX_H
#define __MATRIX_H

#include "typedefs.h"


/* index like:   ptr = (data + (row_indx * cols) + col_indx)   */

struct _Matrix {
    int rows, cols;
    Number **data;
};

Matrix * newMatrix();
void freeMatrix(Matrix *);

char * printMatrix(Matrix *);
char * printMatrixShort(Matrix *);
void setMatrix(Matrix *, Number *, int, int);
Matrix * setMatrixMatrix(Matrix *, Matrix *);

Matrix * negMatrix(Matrix *);
Matrix * invMatrix(Matrix *);

Matrix * mulMatrix(Matrix *, Matrix *);
Matrix * mulMatrixCmplx(Matrix *, Cmplx *);
Matrix * mulMatrixReal(Matrix *, Real *);

Matrix * divMatrix(Matrix *, Matrix *);
Matrix * divMatrixCmplx(Matrix *, Cmplx *);
Matrix * divMatrixReal(Matrix *, Real *);
Matrix * divCmplxMatrix(Cmplx *, Matrix *);
Matrix * divRealMatrix(Real *, Matrix *);

Matrix * modMatrix(Matrix *, Matrix *);
Matrix * modMatrixCmplx(Matrix *, Cmplx *);
Matrix * modMatrixReal(Matrix *, Real *);
Matrix * modCmplxMatrix(Cmplx *, Matrix *);
Matrix * modRealMatrix(Real *, Matrix *);

Matrix * addMatrix(Matrix *, Matrix *);
Matrix * addMatrixCmplx(Matrix *, Cmplx *);
Matrix * addMatrixReal(Matrix *, Real *);

Matrix * subMatrix(Matrix *, Matrix *);
Matrix * subMatrixCmplx(Matrix *, Cmplx *);
Matrix * subMatrixReal(Matrix *, Real *);
Matrix * subCmplxMatrix(Cmplx *, Matrix *);
Matrix * subRealMatrix(Real *, Matrix *);

Matrix * lshiftMatrixReal(Matrix *, Real *);
Matrix * rshiftMatrixReal(Matrix *, Real *);

#endif
