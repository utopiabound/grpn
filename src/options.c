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
/* options.c by Paul Wilkins  1/17/99 */


#include <stdio.h>
#include <gtk/gtk.h>

#include "options.h"
#include "mode.h"
#include "lcd.h"

extern void baseToggleCB(GtkWidget *, gpointer);
extern void radixToggleCB(gpointer);
extern void cmodeToggleCB(gpointer);
extern void dmodeToggleCB(gpointer);

GtkWidget *optBinBtn;
GtkWidget *optOctBtn;
GtkWidget *optDecBtn;
GtkWidget *optDecEngBtn;
GtkWidget *optHexBtn;
GtkWidget *optWindow = NULL;

/* pops up a new window with all the options in it */
void optionsCB(gpointer data){

   GSList *group;
   GtkWidget *button;
   GtkWidget *frame;
   GtkWidget *hbox, *vbox;

   if (!optWindow) {

      optWindow = gtk_window_new (GTK_WINDOW_TOPLEVEL);
      gtk_window_set_title (GTK_WINDOW (optWindow), "Options");

      gtk_signal_connect(GTK_OBJECT(optWindow), "destroy",
			GTK_SIGNAL_FUNC(gtk_widget_destroyed),
			&optWindow);
#ifdef GTK_VER_1_1
      gtk_container_set_border_width(GTK_CONTAINER(optWindow), 5);
#else
      gtk_container_border_width(GTK_CONTAINER(optWindow), 5);
#endif

      /* all the sections go in the vbox */
      vbox = gtk_vbox_new(FALSE, 0);
      gtk_container_add(GTK_CONTAINER(optWindow), vbox);
      gtk_widget_show(vbox);

      /************************************************************/
      frame = gtk_frame_new("Base");
      gtk_box_pack_start(GTK_BOX(vbox), frame, TRUE, TRUE, 0);
#ifdef GTK_VER_1_1
      gtk_container_set_border_width(GTK_CONTAINER(frame), 5);
#else
      gtk_container_border_width(GTK_CONTAINER(frame), 5);
#endif
      gtk_widget_show(frame);

      hbox = gtk_hbox_new(TRUE, 0);
      gtk_container_add(GTK_CONTAINER(frame), hbox);
      gtk_widget_show(hbox);

	 button = gtk_radio_button_new_with_label(NULL, "Bin");
	 gtk_box_pack_start(GTK_BOX(hbox), button, TRUE, TRUE, 0);
         gtk_signal_connect(GTK_OBJECT(button), "toggled",
                                   GTK_SIGNAL_FUNC(baseToggleCB), (gpointer)BINARY);
	 gtk_toggle_button_set_state(GTK_TOGGLE_BUTTON(button), getBaseMode() == BINARY ? TRUE : FALSE);
	 gtk_widget_show(button);
         optBinBtn = button;

	 group = gtk_radio_button_group(GTK_RADIO_BUTTON(button));
	 button = gtk_radio_button_new_with_label(group, "Oct");
	 gtk_box_pack_start(GTK_BOX(hbox), button, TRUE, TRUE, 0);
         gtk_signal_connect(GTK_OBJECT(button), "toggled",
                                   GTK_SIGNAL_FUNC(baseToggleCB), (gpointer)OCTAL);
	 gtk_toggle_button_set_state(GTK_TOGGLE_BUTTON(button), getBaseMode() == OCTAL ? TRUE : FALSE);
	 gtk_widget_show(button);
         optOctBtn = button;

	 group = gtk_radio_button_group(GTK_RADIO_BUTTON(button));
	 button = gtk_radio_button_new_with_label(group, "Dec");
	 gtk_box_pack_start(GTK_BOX(hbox), button, TRUE, TRUE, 0);
         gtk_signal_connect(GTK_OBJECT(button), "toggled",
                                   GTK_SIGNAL_FUNC(baseToggleCB), (gpointer)DECIMAL);
	 gtk_toggle_button_set_state(GTK_TOGGLE_BUTTON(button), getBaseMode() == DECIMAL ? TRUE : FALSE);
	 gtk_widget_show(button);
         optDecBtn = button;

	 group = gtk_radio_button_group(GTK_RADIO_BUTTON(button));
	 button = gtk_radio_button_new_with_label(group, "Eng");
	 gtk_box_pack_start(GTK_BOX(hbox), button, TRUE, TRUE, 0);
         gtk_signal_connect(GTK_OBJECT(button), "toggled",
                                   GTK_SIGNAL_FUNC(baseToggleCB), (gpointer)DECIMAL_ENG);
	 gtk_toggle_button_set_state(GTK_TOGGLE_BUTTON(button), getBaseMode() == DECIMAL_ENG ? TRUE : FALSE);
	 gtk_widget_show(button);
         optDecEngBtn = button;

	 group = gtk_radio_button_group(GTK_RADIO_BUTTON(button));
	 button = gtk_radio_button_new_with_label(group, "Hex");
	 gtk_box_pack_start(GTK_BOX(hbox), button, TRUE, TRUE, 0);
         gtk_signal_connect(GTK_OBJECT(button), "toggled",
                                   GTK_SIGNAL_FUNC(baseToggleCB), (gpointer)HEXIDECIMAL);
	 gtk_toggle_button_set_state(GTK_TOGGLE_BUTTON(button), getBaseMode() == HEXIDECIMAL ? TRUE : FALSE);
	 gtk_widget_show(button);
         optHexBtn = button;

      /************************************************************/
      frame = gtk_frame_new("Radix");
      gtk_box_pack_start(GTK_BOX(vbox), frame, TRUE, TRUE, 0);
#ifdef GTK_VER_1_1
      gtk_container_set_border_width(GTK_CONTAINER(frame), 5);
#else
      gtk_container_border_width(GTK_CONTAINER(frame), 5);
#endif
      gtk_widget_show(frame);

      hbox = gtk_hbox_new(TRUE, 0);
      gtk_container_add(GTK_CONTAINER(frame), hbox);
      gtk_widget_show(hbox);

	 button = gtk_radio_button_new_with_label(NULL, "Degrees");
	 gtk_box_pack_start(GTK_BOX(hbox), button, TRUE, TRUE, 0);
         gtk_signal_connect(GTK_OBJECT(button), "toggled",
                                   GTK_SIGNAL_FUNC(radixToggleCB), (gpointer)DEGREES);
	 gtk_toggle_button_set_state(GTK_TOGGLE_BUTTON(button), getRadixMode() == DEGREES ? TRUE : FALSE);
	 gtk_widget_show(button);

	 group = gtk_radio_button_group(GTK_RADIO_BUTTON(button));
	 button = gtk_radio_button_new_with_label(group, "Radians");
	 gtk_box_pack_start(GTK_BOX(hbox), button, TRUE, TRUE, 0);
         gtk_signal_connect(GTK_OBJECT(button), "toggled",
                                   GTK_SIGNAL_FUNC(radixToggleCB), (gpointer)RADIANS);
	 gtk_toggle_button_set_state(GTK_TOGGLE_BUTTON(button), getRadixMode() == RADIANS ? TRUE : FALSE);
	 gtk_widget_show(button);


      /************************************************************/
      frame = gtk_frame_new("Coordinate Mode");
      gtk_box_pack_start(GTK_BOX(vbox), frame, TRUE, TRUE, 0);
#ifdef GTK_VER_1_1
      gtk_container_set_border_width(GTK_CONTAINER(frame), 5);
#else
      gtk_container_border_width(GTK_CONTAINER(frame), 5);
#endif
      gtk_widget_show(frame);

      hbox = gtk_hbox_new(TRUE, 0);
      gtk_container_add(GTK_CONTAINER(frame), hbox);
      gtk_widget_show(hbox);

	 button = gtk_radio_button_new_with_label(NULL, "Rectangular");
	 gtk_box_pack_start(GTK_BOX(hbox), button, TRUE, TRUE, 0);
         gtk_signal_connect(GTK_OBJECT(button), "toggled",
                                   GTK_SIGNAL_FUNC(cmodeToggleCB), (gpointer)RECTANGULAR);
	 gtk_toggle_button_set_state(GTK_TOGGLE_BUTTON(button), getPolarMode() == RECTANGULAR ? TRUE : FALSE);
	 gtk_widget_show(button);

	 group = gtk_radio_button_group(GTK_RADIO_BUTTON(button));
	 button = gtk_radio_button_new_with_label(group, "Polar");
	 gtk_box_pack_start(GTK_BOX(hbox), button, TRUE, TRUE, 0);
         gtk_signal_connect(GTK_OBJECT(button), "toggled",
                                   GTK_SIGNAL_FUNC(cmodeToggleCB), (gpointer)POLAR);
	 gtk_toggle_button_set_state(GTK_TOGGLE_BUTTON(button), getPolarMode() == POLAR ? TRUE : FALSE);
	 gtk_widget_show(button);


      /************************************************************/
      frame = gtk_frame_new("Display Mode");
      gtk_box_pack_start(GTK_BOX(vbox), frame, TRUE, TRUE, 0);
#ifdef GTK_VER_1_1
      gtk_container_set_border_width(GTK_CONTAINER(frame), 5);
#else
      gtk_container_border_width(GTK_CONTAINER(frame), 5);
#endif
      gtk_widget_show(frame);

      hbox = gtk_hbox_new(TRUE, 0);
      gtk_container_add(GTK_CONTAINER(frame), hbox);
      gtk_widget_show(hbox);

	 button = gtk_radio_button_new_with_label(NULL, "Short");
	 gtk_box_pack_start(GTK_BOX(hbox), button, TRUE, TRUE, 0);
         gtk_signal_connect(GTK_OBJECT(button), "toggled",
                                   GTK_SIGNAL_FUNC(dmodeToggleCB), (gpointer)SHORT_DISPLAY);
	 gtk_toggle_button_set_state(GTK_TOGGLE_BUTTON(button), getLCDDispMode() == SHORT_DISPLAY ? TRUE : FALSE);
	 gtk_widget_show(button);

	 group = gtk_radio_button_group(GTK_RADIO_BUTTON(button));
	 button = gtk_radio_button_new_with_label(group, "Long");
	 gtk_box_pack_start(GTK_BOX(hbox), button, TRUE, TRUE, 0);
         gtk_signal_connect(GTK_OBJECT(button), "toggled",
                                   GTK_SIGNAL_FUNC(dmodeToggleCB), (gpointer)LONG_DISPLAY);
	 gtk_toggle_button_set_state(GTK_TOGGLE_BUTTON(button), getLCDDispMode() == LONG_DISPLAY ? TRUE : FALSE);
	 gtk_widget_show(button);

      /************************************************************/

      button = gtk_button_new_with_label("Dismiss");
      gtk_signal_connect_object(GTK_OBJECT(button), "clicked",
                                 GTK_SIGNAL_FUNC(gtk_widget_destroy),
                                 GTK_OBJECT(optWindow));
      gtk_box_pack_start(GTK_BOX(vbox), button, TRUE, TRUE, 0);
      GTK_WIDGET_SET_FLAGS(button, GTK_CAN_DEFAULT);
      gtk_widget_show(button);


   }

   if (!GTK_WIDGET_VISIBLE(optWindow)) {
      gtk_widget_show(optWindow);
   } else {
      gtk_widget_destroy(optWindow);
   }


}


