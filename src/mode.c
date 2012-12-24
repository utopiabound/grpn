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
/* setup mode.c  by Paul Wilkins  2/8/98 */

#include <stdio.h>
#include <string.h>
#include <gtk/gtk.h>

#include "mode.h"
#include "number.h"
#include "complex.h"

GtkWidget *ModeDisplay = NULL;

int radixMode = RADIANS;
int polarMode = RECTANGULAR;
int baseMode = DECIMAL;

int getRadixMode(){
   return radixMode;
}
void setRadixMode(int mode){
   if(mode == RADIANS) radixMode = RADIANS;
   else if(mode == DEGREES) radixMode = DEGREES;
   else printf("setRadixMode invalid mode\n");
}

void setPolarMode(int val){
   polarMode = val;
}
int getPolarMode(){
   return polarMode;
}


void setBaseMode(int val){
   baseMode = val;
}
int getBaseMode(){
   return baseMode;
}


char *getModeDisplayString(){
   int rad_mode, pol_mode, base_mode;
   static char buf[256];

   rad_mode = getRadixMode();
   switch(rad_mode){
      case DEGREES:
         strcpy(buf, " Degrees   ");
         break;
      case RADIANS:
         strcpy(buf, " Radians   ");
         break;
      default:
         fprintf(stderr, "invalid Radix mode\n");
         break;
   }

   pol_mode = getPolarMode();
   switch(pol_mode){
      case RECTANGULAR:
         strcat(buf, "Rectangular   ");
         break;
      case POLAR:
         strcat(buf, "Polar   ");
         break;
      default:
         fprintf(stderr, "invalid Polar mode\n");
         break;
   }

   base_mode = getBaseMode();
   switch(base_mode){
      case BINARY:
         strcat(buf, "Binary");
         break;
      case OCTAL:
         strcat(buf, "Octal");
         break;
      case DECIMAL:
         strcat(buf, "Decimal");
         break;
      case DECIMAL_ENG:
         strcat(buf, "Decimal-Eng");
         break;
      case HEXIDECIMAL:
         strcat(buf, "Hexidecimal");
         break;
      default:
         fprintf(stderr, "invalid Base mode\n");
         break;
   }

   return buf;
}

void refreshModeDisplay(){
   char *str;

   str = getModeDisplayString();
   gtk_label_set(GTK_LABEL(ModeDisplay), str);

}

GtkWidget *setupModeDisplay(GtkWidget *parent){

   ModeDisplay = gtk_label_new(" ");
   gtk_box_pack_start(GTK_BOX(parent), ModeDisplay, FALSE, FALSE, 0);
   gtk_widget_show(ModeDisplay);


   /* set the string */
   refreshModeDisplay();

   return ModeDisplay;
}
