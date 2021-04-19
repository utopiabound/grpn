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
/* callback_menu.h  by Paul Wilkins 3/15/97 */

#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>

#include "menu.h"
#include "lcd.h"
#include "mode.h"
#include "funcs.h"
#include "help.h"
#include "complex.h"
#include "number.h"
#include "options.h"

gint delete_event(GtkWidget *w, GdkEvent *e, gpointer d) {
    return FALSE;  /* will call our destroy function */
}

void destroy(GtkWidget *widget, gpointer data) {
    gtk_main_quit();
}

/* quit the program */
void quitCB(gpointer data) {
    gtk_main_quit();
}

/* clear the stack */
void clearCB(gpointer data) {

    clearLCD();

    /* refresh the display */
    redrawLCD();

}

void aboutCB(gpointer data) {
    about_popup();
}

void licenseCB(gpointer data) {
    license_popup();
}

void helpCB(gpointer data) {
    help_popup();
}


/* change the base we use to display the numbers */
void baseCB(gpointer clientData) {

    setBaseMode(GPOINTER_TO_INT(clientData));

    /* refresh the display */
    redrawLCD();
    refreshModeDisplay();
}

void baseCmdCB(GtkWidget *w, gpointer clientData) {
    FuncInfo *fi = (FuncInfo *)clientData;

    /* store the mode */
    baseCB(fi->data);

    /* TODO: set the toggles in the buttons */
    if(optWindow){
	gtk_toggle_button_set_state(GTK_TOGGLE_BUTTON(optHexBtn),
				    getBaseMode() == HEXADECIMAL);
	gtk_toggle_button_set_state(GTK_TOGGLE_BUTTON(optDecBtn),
				    getBaseMode() == DECIMAL);
	gtk_toggle_button_set_state(GTK_TOGGLE_BUTTON(optDecEngBtn),
				    getBaseMode() == DECIMAL_ENG);
	gtk_toggle_button_set_state(GTK_TOGGLE_BUTTON(optOctBtn),
				    getBaseMode() == OCTAL);
	gtk_toggle_button_set_state(GTK_TOGGLE_BUTTON(optBinBtn),
				    getBaseMode() == BINARY);
    }
}

void baseToggleCB(GtkWidget *widget, gpointer data) {
    if (GTK_TOGGLE_BUTTON(widget)->active) {
	baseCB(data);
    }
}


/* change the radix mode we use to display the numbers */
void radixCB(gpointer clientData) {
   
    setRadixMode(GPOINTER_TO_INT(clientData));

    /* refresh the display */
    redrawLCD();
    refreshModeDisplay();
}

void radixCmdCB(GtkWidget *w, gpointer clientData) {
    FuncInfo *fi = (FuncInfo *)clientData;

    /* store the mode */
    radixCB(fi->data);
}

void radixToggleCB(GtkWidget *widget, gpointer data) {
    if (GTK_TOGGLE_BUTTON(widget)->active) {
	radixCB(data);
    }
}

/* change the coordinate mode we use to display the numbers */
void cmodeCB(gpointer clientData) {
   
    switch(GPOINTER_TO_INT(clientData)) {
    case RECTANGULAR:
	setPolarMode(RECTANGULAR);
	break;
    case POLAR:
	setPolarMode(POLAR);
	break;
    default:
	fprintf(stderr, "Error: cmodeCB: i shouldn't get here\n");
	exit(0);
    }

    /* refresh the display */
    redrawLCD();
    refreshModeDisplay();
}

void cmodeCmdCB(GtkWidget *w, gpointer clientData) {
    FuncInfo *fi = (FuncInfo *)clientData;

    /* store the mode */
    cmodeCB(fi->data);
}

void cmodeToggleCB(GtkWidget *widget, gpointer data) {
    if (GTK_TOGGLE_BUTTON(widget)->active) {
	cmodeCB(data);
    }
}

/* change the display mode we use to display the numbers */
void dmodeCB(gpointer clientData) {
   
    setLCDDispMode(GPOINTER_TO_INT(clientData));

    /* refresh the display */
    redrawLCD();
}

void dmodeToggleCB(GtkWidget *widget, gpointer data) {
    if (GTK_TOGGLE_BUTTON(widget)->active) {
	dmodeCB(data);
    }
}
