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
/* callback_buttons.c  by Paul Wilkins */

#include <stdio.h>
#include <gtk/gtk.h>

#include "buttons.h"
#include "funcs.h"
#include "editor.h"
#include "error.h"
#include "lcd.h"


/* the generic button callback function */
void genericButtonCB(GtkWidget *w, gpointer clientData){
   FuncInfo *fi = (FuncInfo *)clientData;
   void (*func)();

   /* reset any error string */
   resetError();

   /* call the real work function */
   if(fi->data){
      func = fi->data;
      func();
      redrawLCD();
   }
}

/* the null button callback function */
void nullButtonCB(GtkWidget *w, gpointer clientData){
   printf("Sorry: Unimpilmented feature\n");
}

/* inserts a char into the edit buffer */
void enterNumCB(GtkWidget *w, gpointer clientData){
   int tmp;
   FuncInfo *fi = (FuncInfo *)clientData;

   /* reset any error string */
   resetError();

   tmp = (int)fi->data;
   insertEditor(tmp);
   redrawLCD();
}


/* the +/- key needs a special callback */
/* this is unnessisarily complicated */
void plusMinusCB(GtkWidget *w, gpointer clientData){
   char *p, *line;
   int i, pos, foundE;

   /* reset any error string */
   resetError();

   if(isEditingEditor()){
      line = getLineEditor();

      /* look for an 'e' or 'E' */
      foundE = 0;
      for(p=line; *p!='\0'; p++){
         if(*p == 'e' || *p == 'E'){
            foundE = 1;
            break;
         }
      }

      pos = cursorPosEditor();

      /* if we found an exponent */
      if(foundE){
         p++;

         /* put the cursor after the 'e' or 'E' */
         for(i=(p-line)-pos; i>0; i--) rightEditor();
         for(i=pos-(p-line); i>0; i--) leftEditor();

         if(*p == '+'){
            rightEditor();
            deleteEditor();
            insertEditor('-');
            if(pos-(p-line) > 0) pos--;
         } else if(*p == '-'){
            rightEditor();
            deleteEditor();
            insertEditor('+');
            if(pos-(p-line) > 0) pos--;
         } else {
            insertEditor('-');
            pos++;
         }

         /* restore the cursor to where it should be */
         for(i=(p-line)-pos+1; i>0; i--) leftEditor();
         for(i=pos-(p-line); i>0; i--) rightEditor();
       
      } 
 
      /* we didn't find an exponent */
      else {
         homeEditor();
         if(*line == '+'){
            rightEditor();
            deleteEditor();
            insertEditor('-');
            pos--;
         } else if(*line == '-'){
            rightEditor();
            deleteEditor();
            insertEditor('+');
            pos--;
         } else {
            insertEditor('-');
         }
         for(i=0; i<pos; i++) rightEditor();
      }
   } else {

      NegStack();
   }

   redrawLCD();
}

