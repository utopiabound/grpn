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
/* stack.c  by Paul Wilkins */

#include <stdio.h>
#include <stdlib.h>

#include "stack.h"
#include "number.h"

#define STACK_CHUNK_SIZE 20

/* a stack element */
struct StackElem {
   struct StackElem *t;   /* the node above us */
   struct StackElem *b;   /* the node below us */
   Number *data;          /* the data */
};


/* we build the stack in these arrays */
struct StackChunk {
   int freeIndx;
   struct StackElem *freeList[STACK_CHUNK_SIZE];
   struct StackChunk *next;
   struct StackElem ary[STACK_CHUNK_SIZE];
};

/* The main stack of GRPN.  This is where we store the numbers */
struct Stack stack;

struct StackChunk *stackChunkHead = NULL;

struct Stack *getStack(){
   return &stack;
}

/* get the length of the stack */
int stackLen(){
   return stack.length;
}

/* malloc a new stack chunk */
struct StackChunk * newStackChunk(){
   int i;
   struct StackChunk *c;

   /* malloc a new chunk */
   if(NULL == (c=(struct StackChunk *)malloc(sizeof(struct StackChunk)))){
      perror("Malloc");
      exit(0);
   }

   /* initalize stuff */
   c->freeIndx = STACK_CHUNK_SIZE - 1;
   c->next = NULL;

   for(i=0; i<STACK_CHUNK_SIZE; i++){
      c->freeList[i] = &((c->ary)[i]);
      c->ary[i].data = NULL;
   }

   return c;
}

/* malloc a new stack element */
struct StackElem * newStackEle(){
   struct StackElem *s;
   struct StackChunk *c;

   /* find the first stack chunk with free elements */
   for(c=stackChunkHead; c->freeIndx==-1; c=c->next);

   /* get the StackEle */
   s = c->freeList[c->freeIndx];

   c->freeIndx--;

   /* if there are no more free elements */
   if(c->freeIndx < 0){

      c->freeIndx = -1;

      if(c->next == NULL) c->next = newStackChunk();
   }
 
   return s;
}

void stackAddToFreeList(struct StackElem *s){
   struct StackChunk *c;

   for(c=stackChunkHead; c!=NULL; c=c->next)
      if(s >= c->ary && s <= &((c->ary)[STACK_CHUNK_SIZE]))
         c->freeList[++(c->freeIndx)] = s;
}


int copyStack(struct Stack *srcStack, struct Stack *dstStack, int nelts){
   int i;
   struct StackElem *dst, *src;

   for(i=0, src=srcStack->head; 
       i<nelts && src;
       i++, src=src->t){

      /* set the stuff in the newly created stack element */
      dst = newStackEle();
      dst->data = src->data;
      dst->t = dstStack->head;
      dst->b = NULL;

      incRefcntNumber(dst->data);

      /* update the stuff in the ele above us */
      if(dst->t) dst->t->b = dst;

      /* update the dest stack */
      dstStack->head = dst;
      dstStack->length++;

   }

   if(i != nelts){
      fprintf(stderr, "copyStack: Error copying stack.\n");
      return 0;
   }
   return 1;
}


int setup_stack(){

   stack.head = NULL;
   stack.length = 0;

   stackChunkHead = newStackChunk();
   
   return 1;
}

void clearNamedStack(struct Stack *stk){
   struct StackElem *s;

   for(s=stk->head; s; s=s->t){
      decRefcntNumber(s->data);
      freeNumber(s->data);
      stackAddToFreeList(s);
   }
   stk->head = NULL;
   stk->length = 0;

}

void clearStack(){
   clearNamedStack(&stack);
}

void printStack(){
   char *c;
   struct StackElem *s = NULL;
   struct StackElem *p = NULL;

   /* find the top of the stack */
   for(s=stack.head; s; s=s->t) p = s;

   /* print the numbers starting from the top */
   for(s=p; p; p=p->b){
      c = printNumber(p->data);
      printf("%s\n", c);
      free(c);
   }
}

Number * getStackEle(int which){
   int i;
   struct StackElem *s = NULL;

   if(which < 0 || which >= stack.length) return NULL;

   s = stack.head;
   for(i=0; i<which; i++) s = s->t;

   return s->data;
}

int Push(Number *data){

   struct StackElem *s;

   /* set the stuff in the newly created stack element */
   s = newStackEle();
   s->data = data;
   s->t = stack.head;
   s->b = NULL;

   /* increment the ref count of the number */
   incRefcntNumber(data);

   /* update the stuff in the ele above us */
   if(s->t) s->t->b = s;

   /* update the head */
   stack.head = s;

   stack.length++;

   return 1;  /* success */
}

Number * Pop(){
   Number *ptr;
   struct StackElem *s;

   s = stack.head;

   /* nothing to pop */
   if(s == NULL) return NULL;

   /* update the stuff in the ele above us */
   if(s->t){
      s->t->b = NULL;
      stack.head = s->t;
   } 
   else stack.head = NULL;
   
   ptr = s->data;
   
   /* decrement the ref count of the number */
   decRefcntNumber(ptr);

   stackAddToFreeList(s);

   stack.length--;

   return ptr;
}
