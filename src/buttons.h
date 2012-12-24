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
/* buttons.h   by Paul Wilkins */

#ifndef __BUTTONS_H
#define __BUTTONS_H

#include "funcs.h"

/* size of a button in pixels */
#define BUTTON_SIZE 30

struct RowInfo {
   int numBtns;
   int sizeBtns;
   FuncInfo *fi;
};

extern struct RowInfo rowinf[];
extern int NumButtonRows;
extern int NumFunctionRows;

GtkWidget *setupButtons(GtkWidget *parent);

extern void genericButtonCB(GtkWidget *, gpointer);
extern void nullButtonCB(GtkWidget *, gpointer);
extern void enterNumCB(GtkWidget *, gpointer);
extern void plusMinusCB(GtkWidget *, gpointer);


#endif

