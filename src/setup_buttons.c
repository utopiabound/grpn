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
/* setup_buttons.c  by Paul Wilkins */

#include <stdio.h>
#include <gtk/gtk.h>

#include "buttons.h"
#include "funcs.h"
#include "editor.h"



/* create the button widgets */
GtkWidget *createButton(
   GtkWidget *parent, 
   int leftPos, int rightPos,
   int topPos, int bottomPos,
   FuncInfo *fi)
{
   GtkWidget *btn, *label;
   void (*callBack)(GtkWidget *, gpointer);

   /*
   btn = gtk_button_new_with_label(fi->name);
   */

   btn = gtk_button_new();
   label = gtk_label_new(fi->name);
   gtk_misc_set_alignment(GTK_MISC(label), 0.5, 0.5);
   gtk_misc_set_padding(GTK_MISC(label), 0, 0);
   gtk_container_add(GTK_CONTAINER(btn), label);
   gtk_widget_show(label);


   /* the space around the button 
   gtk_container_border_width(GTK_CONTAINER(btn), 10);
   */

   /* when the button is clicked, we call the "callback" function */
   callBack = fi->CB;
   gtk_signal_connect(GTK_OBJECT (btn), "clicked",
	   GTK_SIGNAL_FUNC(callBack), (gpointer)fi);

   gtk_table_attach_defaults(GTK_TABLE(parent), btn, leftPos, rightPos, topPos, bottomPos);

   gtk_widget_show(btn);

   GTK_WIDGET_UNSET_FLAGS(btn, GTK_CAN_FOCUS);

   return btn;
}


/* setup the buttons */
GtkWidget *setupButtons(GtkWidget *parent){
   int i, j;
   int good;
   int numRows, numCols;
   int tWidth;
   GtkWidget *table;

   /* get the number of rows and columns */
   numRows = NumButtonRows;
   numCols = 0;
   for(i=0; i<numRows; i++){
      if(rowinf[i].numBtns > numCols) numCols = rowinf[i].numBtns;
   }

   /* find the width of the table we need to fit out different sized buttons */
   for(tWidth=numCols; tWidth<100; tWidth++){
      good = 1;
      for(j=0; j<numRows; j++){
         if( (tWidth % rowinf[j].numBtns) != 0){
            good = 0;
            break;
         }
      }
      if(good == 1) break;
   }

   /* create the table that will hold our buttons */
   table = gtk_table_new(numRows, tWidth, TRUE);
   gtk_box_pack_end(GTK_BOX(parent), table, FALSE, FALSE, 0);
   gtk_widget_show(table);

   /* fill the form */
   for(i=0; i<numRows; i++){
      for(j=0; j<rowinf[i].numBtns; j++){
         if(rowinf[i].fi[j].name != NULL){
            createButton(table,
	       tWidth / rowinf[i].numBtns * j,
	       tWidth / rowinf[i].numBtns * (j + 1),
               i, i+1,
               &(rowinf[i].fi[j]) );
         }
      }
   }

   return table;

}
