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
/* editor.c  by Paul Wilkins 3/24/97 */

#include <stdio.h>

#include "editor.h"
#include "funcs.h"
#include "mode.h"

#define EDIT_BUF_LEN 1024

char Ebuf[EDIT_BUF_LEN];     /* the edit buffer */

char * Epos = Ebuf;  /* current cursor position */
char * Eend = Ebuf;  /* end of the line */

int isEditingEditor(){
   if(Ebuf == Eend) return 0;
   return 1;
}

int cursorPosEditor(){
   return (int)(Epos-Ebuf);
}

void setCursorPosEditor(int pos){
   if(pos < 0){
      Epos = Ebuf;
   } else if(pos > Eend-Ebuf){
      Epos = Eend;
   } else {
      Epos = Ebuf + pos;
   }
}

char getPrevEditor(){
   if(Epos < Ebuf+1) return '\0';
   return *(Epos-1);
}

char * getLineEditor(){
   *Eend = '\0';
   return Ebuf;
}


/* process anything in the edit buffer */
void finishEditor(){
   if(Eend > Ebuf){ 
      *Eend = '\0';  
      readLine(Ebuf); 
   }
   Epos = Eend = Ebuf;
}

void insertEditor(char c){
   char *p;

   for(p=Eend; p>=Epos; p--) *(p+1) = *p;
   *Epos = c;
   Epos++; 
   Eend++;

   /* check for buffer overrun- leave 1 byte for '\0' */
   if(Eend >= Ebuf + EDIT_BUF_LEN - 2) Eend = Ebuf + EDIT_BUF_LEN - 2;
   if(Epos >= Ebuf + EDIT_BUF_LEN - 2) Epos = Ebuf + EDIT_BUF_LEN - 3;
}

void deleteEditor(){
   char *p;

   if(Epos == Ebuf) return;
   for(p=Epos; p<Eend; p++) *(p-1) = *p;
   Epos--; Eend--;
}


void homeEditor(){
   Epos = Ebuf;
}

void endEditor(){
   Epos = Eend;
}

void rightEditor(){
   Epos++;
   if(Epos > Eend) Epos = Eend;
}

void leftEditor(){
   Epos--;
   if(Epos < Ebuf) Epos = Ebuf;
}

void cancelEditor(){
   Eend = Epos = Ebuf;
   *Ebuf = '\0';
}

