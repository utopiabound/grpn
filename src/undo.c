/*

Copyright (C)  2000 Paul Wilkins

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
/* undo.c  by Paul Wilkins */

#include <stdio.h>

#include "undo.h"
#include "error.h"
#include "stack.h"
#include "number.h"

/* must be at least 2 */
#define MAX_UNDO_LEVELS 10


int nSavedItems[MAX_UNDO_LEVELS];
int nUndoItems[MAX_UNDO_LEVELS];
struct Stack undoStacks[MAX_UNDO_LEVELS];

int ringIndx;
int undoLevel;

void initUndoSystem(){
   int i;
   static int initialized = 0;

   if(initialized != 0){
      for(i=0; i<MAX_UNDO_LEVELS; i++){
	 clearNamedStack(&(undoStacks[i]));
      }
   }
   initialized = 1;

   for(i=0; i<MAX_UNDO_LEVELS; i++){
      nSavedItems[i] = 0;
      nUndoItems[i] = 0;
      undoStacks[i].head = NULL;
      undoStacks[i].length = 0;
   }
   ringIndx = 0;
   undoLevel = 0;
}


void UndoStack(){
   int i;

   if(undoLevel == 0){
      setStringError("Undo Error: Nothing to Undo.");
      return;
   }

   ringIndx--;
   if(ringIndx < 0) ringIndx = MAX_UNDO_LEVELS - 1;

   undoLevel--;
   if(undoLevel <= 0) undoLevel = 0;

   /* throw away the stuff we are undoing */
   for(i=0; i<nUndoItems[ringIndx]; i++){
      freeNumber(Pop());
   }

   /* restore the origional numbers */
   copyStack(&(undoStacks[ringIndx]), getStack(), nSavedItems[ringIndx]);

   /* clear this stack */
   clearNamedStack(&(undoStacks[ringIndx]));
   nSavedItems[ringIndx] = 0;

}

/* Whoops, we called SaveStackState but didn't mean to */
void CancelStackState(){

   nSavedItems[ringIndx] = 0;

   /* clear this stack */
   clearNamedStack(&(undoStacks[ringIndx]));

}

/* Called before anything that modifies the stack.
 * Stores args worth of numbers from the stack which will be
 *    used to restore tohe stack if UndoStack is called.
 */
void SaveStackState(int args){

   /* make space for the new saved stack */
   if(undoLevel >= MAX_UNDO_LEVELS){
      clearNamedStack(&(undoStacks[ringIndx]));
   }

   /* save the new info */
   nSavedItems[ringIndx] = args;
   copyStack(getStack(), &(undoStacks[ringIndx]), args);

}
void AddStackState(int args){
   /* save the new info */
   nSavedItems[ringIndx] += args;
   copyStack(getStack(), &(undoStacks[ringIndx]), args);
}

/* Called after anything that modifies the stack.
 * Records the number of stack elements that must be poped
 *    from the stack to undo the last operation if UndoStack
 *    is called.
 */
void UndoStackState(int args){

   nUndoItems[ringIndx] = args;

   undoLevel++;
   if(undoLevel >= MAX_UNDO_LEVELS) undoLevel = MAX_UNDO_LEVELS;

   ringIndx++;
   if(ringIndx >= MAX_UNDO_LEVELS) ringIndx = 0;
}
