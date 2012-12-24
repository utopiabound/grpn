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

#include <stdio.h>
#include <stdlib.h>
#ifdef USE_GNOME
#include <gnome.h>
#endif
#include <gtk/gtk.h>

#include "menu.h"
#include "mode.h"
#include "options.h"

extern void quitCB(gpointer);
extern void clearCB(gpointer);
extern void optionsCB(gpointer);
extern void licenseCB(gpointer);
extern void helpCB(gpointer);


#ifdef OPTIONS_MENU
extern void baseCB(gpointer);
extern void radixCB(gpointer);
extern void cmodeCB(gpointer);
extern void dmodeCB(gpointer);
#endif


#ifdef USE_GNOME

GnomeUIInfo filemenu[] = {
   {GNOME_APP_UI_ITEM, 
      "Clear", "Clear and reset GRPN",
      clearCB, NULL, NULL, 
      GNOME_APP_PIXMAP_NONE, NULL, 0, 0, NULL},
   {GNOME_APP_UI_ITEM, 
      "Options", "Options",
      optionsCB, NULL, NULL, 
      GNOME_APP_PIXMAP_NONE, NULL, 0, 0, NULL},
   GNOMEUIINFO_MENU_EXIT_ITEM(quitCB, NULL),
   GNOMEUIINFO_END
};
GnomeUIInfo helpmenu[] = {
   {GNOME_APP_UI_ITEM, 
      "License", "License",
      licenseCB, NULL, NULL, 
      GNOME_APP_PIXMAP_NONE, NULL, 0, 0, NULL},
   GNOMEUIINFO_HELP("grpn"),
   GNOMEUIINFO_END
};
GnomeUIInfo mainmenu[] = {
   GNOMEUIINFO_MENU_FILE_TREE(filemenu),
   GNOMEUIINFO_MENU_HELP_TREE(helpmenu),
   GNOMEUIINFO_END
};


/* create the menubar */
GtkWidget *setup_menu(GtkWidget *parent)
{
   gnome_app_create_menus(GNOME_APP(parent), mainmenu);

   return NULL;
}

#else /* not USE_GNOME */

/* create the menubar */
GtkWidget *setup_menu(GtkWidget *parent)
{
   GtkWidget *menu_bar;
   GtkWidget    *fileM, *fileI;
   GtkWidget       *clearI;
   GtkWidget       *optionsI;
   GtkWidget       *quitI;
#ifdef OPTIONS_MENU
   GtkWidget    *optM, *optI;
   GtkWidget       *baseM, *baseI;
   GtkWidget          *binI;
   GtkWidget          *octI;
   GtkWidget          *decI;
   GtkWidget          *hexI;
   GtkWidget       *radixM, *radixI;
   GtkWidget          *degI;
   GtkWidget          *radI;
   GtkWidget       *coordM, *coordI;
   GtkWidget          *rectI;
   GtkWidget          *polI;
   GtkWidget       *dispM, *dispI;
   GtkWidget          *shrtI;
   GtkWidget          *longI;
#endif
   GtkWidget    *helpM, *helpI;
   GtkWidget       *hlpI;
   GtkWidget       *licenseI;



   /********************************************************/

   /* File */
   fileM = gtk_menu_new(); /* Remember: don't gtk_widget_show the menu */

      /* Clear */
      clearI = gtk_menu_item_new_with_label("Clear");
      gtk_menu_append(GTK_MENU(fileM), clearI);
      gtk_widget_show(clearI);
      gtk_signal_connect_object(GTK_OBJECT(clearI), "activate",
				GTK_SIGNAL_FUNC(clearCB), NULL);

      /* Options */
      optionsI = gtk_menu_item_new_with_label("Options");
      gtk_menu_append(GTK_MENU(fileM), optionsI);
      gtk_widget_show(optionsI);
      gtk_signal_connect_object(GTK_OBJECT(optionsI), "activate",
				GTK_SIGNAL_FUNC(optionsCB), NULL);
      /* Quit */
      quitI = gtk_menu_item_new_with_label("Quit");
      gtk_menu_append(GTK_MENU(fileM), quitI);
      gtk_widget_show(quitI);
      gtk_signal_connect_object(GTK_OBJECT(quitI), "activate",
				GTK_SIGNAL_FUNC(quitCB), NULL);


   /********************************************************/
#ifdef OPTIONS_MENU
   /* create the "Options" pulldown menu */
   optM = gtk_menu_new();

	 /* Base */
	 baseM = gtk_menu_new();

	 /* Binary */
	 binI = gtk_menu_item_new_with_label("Binary");
	 gtk_menu_append(GTK_MENU(baseM), binI);
	 gtk_widget_show(binI);
	 gtk_signal_connect_object(GTK_OBJECT(binI), "activate",
				   GTK_SIGNAL_FUNC(baseCB), (gpointer)BINARY);
	 /* Octal */
	 octI = gtk_menu_item_new_with_label("Octal");
	 gtk_menu_append(GTK_MENU(baseM), octI);
	 gtk_widget_show(octI);
	 gtk_signal_connect_object(GTK_OBJECT(octI), "activate",
				   GTK_SIGNAL_FUNC(baseCB), (gpointer)OCTAL);
	 /* Decimal */
	 decI = gtk_menu_item_new_with_label("Decimal");
	 gtk_menu_append(GTK_MENU(baseM), decI);
	 gtk_widget_show(decI);
	 gtk_signal_connect_object(GTK_OBJECT(decI), "activate",
				   GTK_SIGNAL_FUNC(baseCB), (gpointer)DECIMAL);
	 /* Hex */
	 hexI = gtk_menu_item_new_with_label("Hex");
	 gtk_menu_append(GTK_MENU(baseM), hexI);
	 gtk_widget_show(hexI);
	 gtk_signal_connect_object(GTK_OBJECT(hexI), "activate",
				   GTK_SIGNAL_FUNC(baseCB), (gpointer)HEXIDECIMAL);

      baseI = gtk_menu_item_new_with_label("Base");
      gtk_menu_append(GTK_MENU(optM), baseI);
      gtk_widget_show(baseI);
      gtk_menu_item_set_submenu(GTK_MENU_ITEM(baseI), baseM);

	 /* Radix */
	 radixM = gtk_menu_new();

	 /* Degrees */
	 degI = gtk_menu_item_new_with_label("Degrees");
	 gtk_menu_append(GTK_MENU(radixM), degI);
	 gtk_widget_show(degI);
	 gtk_signal_connect_object(GTK_OBJECT(degI), "activate",
				   GTK_SIGNAL_FUNC(radixCB), (gpointer)DEGREES);
	 /* Radians */
	 radI = gtk_menu_item_new_with_label("Radians");
	 gtk_menu_append(GTK_MENU(radixM), radI);
	 gtk_widget_show(radI);
	 gtk_signal_connect_object(GTK_OBJECT(radI), "activate",
				   GTK_SIGNAL_FUNC(radixCB), (gpointer)RADIANS);

      radixI = gtk_menu_item_new_with_label("Radix");
      gtk_menu_append(GTK_MENU(optM), radixI);
      gtk_widget_show(radixI);
      gtk_menu_item_set_submenu(GTK_MENU_ITEM(radixI), radixM);

	 /* Coordinate Mode */
	 coordM = gtk_menu_new();

	 /* Rectangular */
	 rectI = gtk_menu_item_new_with_label("Rectangular");
	 gtk_menu_append(GTK_MENU(coordM), rectI);
	 gtk_widget_show(rectI);
	 gtk_signal_connect_object(GTK_OBJECT(rectI), "activate",
				   GTK_SIGNAL_FUNC(cmodeCB), (gpointer)RECTANGULAR);
	 /* Polar */
	 polI = gtk_menu_item_new_with_label("Polar");
	 gtk_menu_append(GTK_MENU(coordM), polI);
	 gtk_widget_show(polI);
	 gtk_signal_connect_object(GTK_OBJECT(polI), "activate",
				   GTK_SIGNAL_FUNC(cmodeCB), (gpointer)POLAR);

      coordI = gtk_menu_item_new_with_label("Coordinate Mode");
      gtk_menu_append(GTK_MENU(optM), coordI);
      gtk_widget_show(coordI);
      gtk_menu_item_set_submenu(GTK_MENU_ITEM(coordI), coordM);

	 /* Display Mode */
	 dispM = gtk_menu_new();

	 /* Short */
	 shrtI = gtk_menu_item_new_with_label("Short");
	 gtk_menu_append(GTK_MENU(dispM), shrtI);
	 gtk_widget_show(shrtI);
	 gtk_signal_connect_object(GTK_OBJECT(shrtI), "activate",
				   GTK_SIGNAL_FUNC(dmodeCB), (gpointer)SHORT_DISPLAY);
	 /* Long */
	 longI = gtk_menu_item_new_with_label("Long");
	 gtk_menu_append(GTK_MENU(dispM), longI);
	 gtk_widget_show(longI);
	 gtk_signal_connect_object(GTK_OBJECT(longI), "activate",
				   GTK_SIGNAL_FUNC(dmodeCB), (gpointer)LONG_DISPLAY);

      dispI = gtk_menu_item_new_with_label("Display Mode");
      gtk_menu_append(GTK_MENU(optM), dispI);
      gtk_widget_show(dispI);
      gtk_menu_item_set_submenu(GTK_MENU_ITEM(dispI), dispM);

#endif

   /********************************************************/

   /* create the "Help" pulldown menu */
   helpM = gtk_menu_new();

      /* Help */
      hlpI = gtk_menu_item_new_with_label("Help");
      gtk_menu_append(GTK_MENU(helpM), hlpI);
      gtk_widget_show(hlpI);
      gtk_signal_connect_object(GTK_OBJECT(hlpI), "activate",
				GTK_SIGNAL_FUNC(helpCB), NULL);
      /* Help */
      licenseI = gtk_menu_item_new_with_label("License");
      gtk_menu_append(GTK_MENU(helpM), licenseI);
      gtk_widget_show(licenseI);
      gtk_signal_connect_object(GTK_OBJECT(licenseI), "activate",
				GTK_SIGNAL_FUNC(licenseCB), NULL);

   /********************************************************/

   /* tell the menubar who is the heplp widget */

   menu_bar = gtk_menu_bar_new();
   gtk_box_pack_start(GTK_BOX(parent), menu_bar, FALSE, FALSE, 0);
   gtk_widget_show(menu_bar);
 
   fileI = gtk_menu_item_new_with_label("File");
   gtk_menu_item_set_submenu(GTK_MENU_ITEM(fileI), fileM);
   gtk_menu_bar_append(GTK_MENU_BAR(menu_bar), fileI);
   gtk_widget_show(fileI);

#ifdef OPTIONS_MENU
   optI = gtk_menu_item_new_with_label("Options");
   gtk_menu_item_set_submenu(GTK_MENU_ITEM(optI), optM);
   gtk_menu_bar_append(GTK_MENU_BAR(menu_bar), optI);
   gtk_widget_show(optI);
#endif

   helpI = gtk_menu_item_new_with_label("Help");
   gtk_menu_item_right_justify(GTK_MENU_ITEM(helpI));
   gtk_menu_item_set_submenu(GTK_MENU_ITEM(helpI), helpM);
   gtk_menu_bar_append(GTK_MENU_BAR(menu_bar), helpI);
   gtk_widget_show(helpI);

   return menu_bar;
}
#endif  /* USE_GNOME */

