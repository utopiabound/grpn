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
/* stack.h by Paul Wilkins  3/15/97 */

#ifndef __STACK_H
#define __STACK_H

#include "typedefs.h"

struct StackElem;
struct Stack {
    int length;
    /* the head (bottom) of the stack */
    struct StackElem *head;
};


int setup_stack();
int stackLen();
void printStack();

void clearStack();
void clearNamedStack(struct Stack *);

struct Stack *getStack();
int copyStack(struct Stack *, struct Stack *, int);

Number * getStackEle(int);
int Push(Number *);
Number * Pop();

#endif
