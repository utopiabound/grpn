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
/* funcs.c  by Paul Wilkins */

#include <stdio.h>
#include <string.h>
#include <gtk/gtk.h>

#include "stack.h"
#include "number.h"
#include "funcs.h"
#include "undo.h"
#include "constant.h"
#include "editor.h"
#include "run_cmd.h"
#include "error.h"
#include "mode.h"

void decomposeMtrx();
void createMtrx();
Number * readNumber(char *line, int *nread);
int isDigitBase(char c);

void readLine(char *line){
   int pos;  /* the number of characters in the number */
   Number *n1;

   if((*line != '"' && *line != '\'') && (isDigitBase(*line) || *line == '-' || *line == '+' || *line == '.')){

      /* try to read in a number */
      if(NULL == (n1 = readNumber(line, &pos))){
	 setStringError("Error: Cannot read number.");
         return;
      }

      SaveStackState(0);  /* what to restore to the stack after an undo */
      Push(n1);
      UndoStackState(1);  /* what to undo from the stack after an undo */

      /* what's left must be a command */
      if(strlen(line) > pos) runCmd(line+pos);

   } else {

      /* this is a command */
      runCmd(line);
   }
}



void DupStack(){
   Number *n1;

   finishEditor();

   if(checkArgs("Dup", 1)) return;
   SaveStackState(0);  /* what to restore to the stack after an undo */

   n1 = getStackEle(0);
   Push(setNumberNumber(newNumber(), n1));

   UndoStackState(1);  /* what to undo from the stack after an undo */
}

void PopStack(){
   if(checkArgs("Drop", 1)) return;
   SaveStackState(1);

   freeNumber(Pop());

   UndoStackState(0);
}

void PushStack(){
   if(isEditingEditor()) finishEditor();
   else DupStack();

}

void PiStack(){
   Number *n1;

   finishEditor();

   SaveStackState(0);

   n1 = setNumberReal(newNumber(), realPi);
   Push(n1);

   UndoStackState(1);
}

void NegStack(){
   Number *n1, *n2;

   finishEditor();

   if(checkArgs("+/-", 1)) return;
   SaveStackState(1);

   n2 = negNumber((n1=Pop()));
   if(n2){
      Push(n2); freeNumber(n1);
      UndoStackState(1);
   } else {
      Push(n1); 
      CancelStackState();
   }
}

void InvStack(){
   Number *n1, *n2;

   finishEditor();

   if(checkArgs("Inv", 1)) return;
   SaveStackState(1);

   n2 = invNumber((n1=Pop()));
   if(n2){
      Push(n2); freeNumber(n1);
      UndoStackState(1);
   } else {
      Push(n1); 
      CancelStackState();
   }
}


void LnStack(){
   Number *n1, *n2;

   finishEditor();

   if(checkArgs("ln", 1)) return;
   SaveStackState(1);

   n2 = lnNumber((n1=Pop()));
   if(n2){
      Push(n2); freeNumber(n1);
      UndoStackState(1);
   } else {
      Push(n1); 
      CancelStackState();
   }
}

void LogStack(){
   Number *n1, *n2;

   finishEditor();

   if(checkArgs("log", 1)) return;
   SaveStackState(1);

   n2 = logNumber((n1=Pop()));
   if(n2){
      Push(n2); freeNumber(n1);
      UndoStackState(1);
   } else {
      Push(n1); 
      CancelStackState();
   }
}

void TenxStack(){
   Number *n1, *n2, *n3;

   finishEditor();

   if(checkArgs("10^x", 1)) return;
   SaveStackState(1);

   n3 = setNumberReal(newNumber(), realTen);

   n2 = powNumber(n3, (n1=Pop()));
   freeNumber(n3);

   if(n2){
      Push(n2); freeNumber(n1);
      UndoStackState(1);
   } else {
      Push(n1);
      CancelStackState();
   }
}

void ExpStack(){
   Number *n1, *n2;

   finishEditor();

   if(checkArgs("exp", 1)) return;
   SaveStackState(1);

   n2 = expNumber((n1=Pop()));
   if(n2){
      Push(n2); freeNumber(n1);
      UndoStackState(1);
   } else {
      Push(n1); 
      CancelStackState();
   }
}

void PowStack(){
   Number *n1, *n2, *n3;

   finishEditor();

   if(checkArgs("pow", 2)) return;
   SaveStackState(2);

   n1 = Pop();

   n3 = powNumber((n2=Pop()), n1);
   if(n3){
      Push(n3); freeNumber(n1); freeNumber(n2);
      UndoStackState(1);
   } else {
      Push(n2); Push(n1);
      CancelStackState();
   }
}

void NrootStack(){
   Number *n1, *n2, *n3, *n4;

   finishEditor();

   if(checkArgs("nroot", 2)) return;
   SaveStackState(2);

   n4 = invNumber((n1=Pop()));
   if(NULL == n4){
      Push(n1);
      CancelStackState();
      return;
   }

   n3 = powNumber((n2=Pop()), n4);
   freeNumber(n4);
   if(n3){
      Push(n3); freeNumber(n1); freeNumber(n2);
      UndoStackState(1);
   } else {
      Push(n2); Push(n1);
      CancelStackState();
   }
}

void SqrStack(){
   Real *r1;
   Number *n1, *n2, *n3;

   finishEditor();

   if(checkArgs("sqr", 1)) return;
   SaveStackState(1);

   r1 = setRealDouble(newReal(), 2.0);
   n3 = setNumberReal(newNumber(), r1);
   freeReal(r1);

   n2 = powNumber((n1=Pop()), n3);
   freeNumber(n3);

   if(n2){
      Push(n2); freeNumber(n1);
      UndoStackState(1);
   } else {
      Push(n1);
      CancelStackState();
   }
}

void SqrtStack(){
   Number *n1, *n2, *n3;

   finishEditor();

   if(checkArgs("sqrt", 1)) return;
   SaveStackState(1);

   n3 = setNumberReal(newNumber(), realHalf);

   n2 = powNumber((n1=Pop()), n3);
   freeNumber(n3);

   if(n2){
      Push(n2); freeNumber(n1);
      UndoStackState(1);
   } else {
      Push(n1);
      CancelStackState();
   }
}

void Db10Stack(){
   Number *n1, *n2;

   finishEditor();

   if(checkArgs("db10", 1)) return;
   SaveStackState(1);

   n1 = Pop();

   n2 = dbNumber(n1, 10.0);
   if(n2){
      Push(n2); freeNumber(n1);
      UndoStackState(1);
   } else {
      Push(n1);
      CancelStackState();
   }
}

void Db20Stack(){
   Number *n1, *n2;

   finishEditor();

   if(checkArgs("db20", 1)) return;
   SaveStackState(1);

   n1 = Pop();

   n2 = dbNumber(n1, 20.0);
   if(n2){
      Push(n2); freeNumber(n1);
      UndoStackState(1);
   } else {
      Push(n1);
      CancelStackState();
   }
}

void RipStack(){
   Number *n1, *n2, *n3;

   finishEditor();

   if(checkArgs("rip", 2)) return;
   SaveStackState(2);

   n1 = Pop();

   n3 = ripNumber((n2=Pop()), n1);
   if(n3){
      Push(n3); freeNumber(n1); freeNumber(n2);
      UndoStackState(1);
   } else {
      Push(n2); Push(n1);
      CancelStackState();
   }
}


void SinStack(){
   Number *n1, *n2;

   finishEditor();

   if(checkArgs("sin", 1)) return;
   SaveStackState(1);

   n2 = sinNumber((n1=Pop()));
   if(n2){
      Push(n2); freeNumber(n1);
      UndoStackState(1);
   } else {
      Push(n1); 
      CancelStackState();
   }
}

void CosStack(){
   Number *n1, *n2;

   finishEditor();

   if(checkArgs("cos", 1)) return;
   SaveStackState(1);

   n2 = cosNumber((n1=Pop()));
   if(n2){
      Push(n2); freeNumber(n1);
      UndoStackState(1);
   } else {
      Push(n1); 
      CancelStackState();
   }
}

void TanStack(){
   Number *n1, *n2;

   finishEditor();

   if(checkArgs("tan", 1)) return;
   SaveStackState(1);

   n2 = tanNumber((n1=Pop()));
   if(n2){
      Push(n2); freeNumber(n1);
      UndoStackState(1);
   } else {
      Push(n1); 
      CancelStackState();
   }
}

void AsinStack(){
   Number *n1, *n2;

   finishEditor();

   if(checkArgs("asin", 1)) return;
   SaveStackState(1);

   n2 = asinNumber((n1=Pop()));
   if(n2){
      Push(n2); freeNumber(n1);
      UndoStackState(1);
   } else {
      Push(n1); 
      CancelStackState();
   }
}

void AcosStack(){
   Number *n1, *n2;

   finishEditor();

   if(checkArgs("acos", 1)) return;
   SaveStackState(1);

   n2 = acosNumber((n1=Pop()));
   if(n2){
      Push(n2); freeNumber(n1);
      UndoStackState(1);
   } else {
      Push(n1); 
      CancelStackState();
   }
}

void AtanStack(){
   Number *n1, *n2;

   finishEditor();

   if(checkArgs("atan", 1)) return;
   SaveStackState(1);

   n2 = atanNumber((n1=Pop()));
   if(n2){
      Push(n2); freeNumber(n1);
      UndoStackState(1);
   } else {
      Push(n1); 
      CancelStackState();
   }
}

void AddStack(){
   Number *n1, *n2, *n3;

   finishEditor();

   if(checkArgs("+", 2)) return;
   SaveStackState(2);

   n1 = Pop();
   n3 = addNumber((n2=Pop()), n1);
   if(n3){
      Push(n3); freeNumber(n1); freeNumber(n2);
      UndoStackState(1);
   } else {
      Push(n2); Push(n1);
      CancelStackState();
   }
}

void SubStack(){
   Number *n1, *n2, *n3;

   finishEditor();

   if(checkArgs("-", 2)) return;
   SaveStackState(2);

   n1 = Pop();
   n3 = subNumber((n2=Pop()), n1);
   if(n3){
      Push(n3); freeNumber(n1); freeNumber(n2);
      UndoStackState(1);
   } else {
      Push(n2); Push(n1);
      CancelStackState();
   }
}

void MulStack(){
   Number *n1, *n2, *n3;

   finishEditor();

   if(checkArgs("*", 2)) return;
   SaveStackState(2);

   n1 = Pop();
   n3 = mulNumber((n2=Pop()), n1);
   if(n3){
      Push(n3); freeNumber(n1); freeNumber(n2);
      UndoStackState(1);
   } else {
      Push(n2); Push(n1);
      CancelStackState();
   }
}

void DivStack(){
   Number *n1, *n2, *n3;

   finishEditor();

   if(checkArgs("/", 2)) return;
   SaveStackState(2);

   n1 = Pop();
   n3 = divNumber((n2=Pop()), n1);
   if(n3){
      Push(n3); freeNumber(n1); freeNumber(n2);
      UndoStackState(1);
   } else {
      Push(n2); Push(n1);
      CancelStackState();
   }
}

void SwapStack(){
   Number *n1, *n2;

   finishEditor();

   if(checkArgs("swap", 2)) return;
   SaveStackState(2);

   n1 = Pop();
   n2 = Pop();
   Push(n1);
   Push(n2);

   UndoStackState(2);
}

void CplxStack(){
   Number *n1, *n2;
   Cmplx *c1;

   finishEditor();

   if(checkArgs("cplx", 2)) return;
   SaveStackState(2);

   n1 = Pop();
   n2 = Pop();
   if(n1->type != REAL || n2->type != REAL){
      setStringError("-> complex Error: Bad Argument Type.");
      Push(n2);
      Push(n1);
      CancelStackState();
      return;
   }
   c1 = inputCmplxReal(newCmplx(), n2->data, n1->data);
   freeNumber(n1); freeNumber(n2);
   Push(setNumberCmplx(newNumber(), c1));
   freeCmplx(c1);

   UndoStackState(1);
}

/* either make a matrix or decompose a matrix */
void MtrxStack(){
   Number *n1;

   finishEditor();

   if(checkArgs("matrix", 1)) return;

   n1 = getStackEle(0);

   switch(n1->type){
      case COMPLEX:
         setStringError("matrix Error: Bad Argument Type.");
         break;
      case MATRIX:
         decomposeMtrx();
         break;
      case REAL:
         createMtrx();
         break;
   }
}


void decomposeMtrx(){
   int i, j, matSize;
   Number *n1, **ptr;
   Real *rs, *cs;
   Matrix *m1;

   if(checkArgs("matrix", 1)) return;
   SaveStackState(1);

   n1 = Pop();
   m1 = (Matrix *)n1->data;
   matSize = m1->rows * m1->cols;

   for(i=0; i<m1->rows; i++){
      for(j=0; j<m1->cols; j++){
         ptr = m1->data + (m1->cols * i) + j;
         Push(setNumberNumber(newNumber(), *ptr));
      }
   }
   Push(setNumberReal(newNumber(), setRealDouble((cs=newReal()), m1->cols)));
   Push(setNumberReal(newNumber(), setRealDouble((rs=newReal()), m1->rows)));
   freeReal(cs);
   freeReal(rs);

   freeNumber(n1);

   UndoStackState(2 + matSize);
}


void createMtrx(){
   int i, j;
   int rows, cols;
   Number *n1, *n2;
   Matrix *m1;

   if(checkArgs("matrix", 2)) return;
   SaveStackState(2);

   n1 = Pop();
   n2 = Pop();
   if(n1->type != REAL || n2->type != REAL){
      setStringError("matrix Error: Bad Argument Type.");
      Push(n2);
      Push(n1);
      CancelStackState();
      return;
   }
   rows = (int)setDoubleReal(n1->data);
   cols = (int)setDoubleReal(n2->data);
   if(rows == 0.0 || cols == 0.0){
      setStringError("matrix Error: Bad Argument Type.");
      Push(n2);
      Push(n1);
      CancelStackState();
      return;
   }

   if(checkArgs("matrix", rows * cols)){
      Push(n2);
      Push(n1);
      CancelStackState();
      return;
   }
   freeNumber(n1); freeNumber(n2);

   AddStackState(rows * cols);

   /* read in the matrix */
   m1 = newMatrix();

   for(i=rows-1; i>=0; i--){
      for(j=cols-1; j>=0; j--){

         setMatrix(m1, (n1=Pop()), i, j);

         freeNumber(n1); 
      }
   }

   Push(setNumberMatrix(newNumber(), m1));

   freeMatrix(m1); 

   UndoStackState(1);
}

#define IS_HEX(c) (\
   (c)=='a' || (c)=='A' || \
   (c)=='b' || (c)=='B' || \
   (c)=='c' || (c)=='C' || \
   (c)=='d' || (c)=='D' || \
   (c)=='e' || (c)=='E' || \
   (c)=='f' || (c)=='F' || \
   isdigit((c)))
#define IS_DEC(c) isdigit((c))
#define IS_OCT(c) (\
   (c)=='0' || (c)=='1' || \
   (c)=='2' || (c)=='3' || \
   (c)=='4' || (c)=='5' || \
   (c)=='6' || (c)=='7')
#define IS_BIN(c) ((c)=='0' || (c)=='1') 

int isDigitBase(char c){
   switch(getBaseMode()){
      case HEXIDECIMAL:
         return IS_HEX(c);
         break;
      case DECIMAL:
         return IS_DEC(c);
         break;
      case OCTAL:
         return IS_OCT(c);
         break;
      case BINARY:
         return IS_BIN(c);
         break;
   }
}


Number * readNumber(char *line, int *nread){
   char *p, buf[2];
   int i1;
   double d1, num;
   Real *r1;
   Number *n1;

   /* read in the number */
   switch(getBaseMode()){
      case HEXIDECIMAL:
         *nread = 0;
         /* look for "0x" */
         if(0 == strncmp(line, "0x", 2)){
            line+=2;
            *nread = 2;
         }
         num = 0.0;
         buf[1] = '\0';
         for(p=line; IS_HEX(*p); p++, (*nread)++){
            buf[0] = *p;
            sscanf(buf, "%x", &i1);
            num *= 16.0;
            num += (double)i1;
         }
         if(p-line == 0) return NULL;
	 break;
      case DECIMAL:
	 /* the line starts with a number-- read it in */
	 if(1 != sscanf(line, "%lg%n", &num, nread)){
	    return NULL;
	 }
	 break;
      case OCTAL:
         *nread = 0;
         num = 0.0;
         buf[1] = '\0';
         for(p=line; IS_OCT(*p); p++, (*nread)++){
            buf[0] = *p;
            sscanf(buf, "%d", &i1);
            num *= 8.0;
            num += (double)i1;
         }
         if(p-line == 0) return NULL;
	 break;
      case BINARY:
         *nread = 0;
         num = 0.0;
         for(p=line; IS_BIN(*p); p++, (*nread)++){
            num *= 2.0;
            if(*p == '1') num += 1.0;
         }
         if(p-line == 0) return NULL;
	 break;
   }


   r1 = setRealDouble(newReal(), num);
   n1 = setNumberReal(newNumber(), r1);
   freeReal(r1);

   return n1;
}


