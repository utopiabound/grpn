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

/* main.c by Paul Wilkins.
 * This file contains the main program.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#ifdef USE_GNOME
#include <gnome.h>
#endif
#include <gtk/gtk.h>

#include "grpn.xbm"
#include "stack.h"
#include "menu.h"
#include "mode.h"
#include "lcd.h"
#include "undo.h"
#include "buttons.h"
#include "constant.h"

void usage(char *str){
    if (str)
	fprintf(stderr, "Error: %s\n", str);
    fprintf(stderr, "Usage: grpn [-fn font] [-btn-fn font] [-disp-fn font] [-bm basemode]\n");
    fprintf(stderr, "            [-rows n] [-cols n] [-b]\n");
    fprintf(stderr, "   -b:        Don't draw the buttons.\n");
    fprintf(stderr, "   -rows:     Display (a minimum of) \"n\" rows.\n");
    fprintf(stderr, "   -cols:     Display (a minimum of) \"n\" columns.\n");
    fprintf(stderr, "   -btn-fn:   Use \"font\" for the buttons and menu-bar.\n");
    fprintf(stderr, "   -disp-fn:  Use \"font\" for the main display.\n");
    fprintf(stderr, "   -fn:       Specifies both -btn-fn and -disp-fn.\n");
    fprintf(stderr, "   -bm:        basemode: dec, eng, bin, oct or hex.\n");
}

GtkWidget *main_w;

int main(int argc, char *argv[])
{
    int n;
    int rows, cols;
    int drawButtons;
    char *btn_font, *disp_font;
    /* GtkWidget *main_w; */
    GtkWidget *vbox;
    GdkBitmap *icon_bitmap;
#ifdef USE_GNOME
    GnomeAppBar *appbar;
#endif

    setlocale(LC_ALL, "C");
#ifdef USE_GNOME
    gnome_init("grpn", "1.0", argc, argv);
    gnome_app_new("grpn", "grpn");
#else
    /* initialize gtk */
    gtk_init(&argc, &argv);
#endif

    /* initial values */
    drawButtons = 1;
    rows = 8;
    cols = 30;
    btn_font = disp_font = NULL;

    /* process command line args */
    n = 1;
    while (n < argc) {
	if (0 == strcmp("-fn", argv[n])) {
	    n++;
	    if(n >= argc){
		usage("Missing required argument for -fn.");
		exit(0);
	    }
	    btn_font = disp_font = argv[n];
	} else if (0 == strcmp("-btn-fn", argv[n])) {
	    n++;
	    if (n >= argc) {
		usage("Missing required argument for -btn-fn.");
		exit(0);
	    }
	    btn_font = argv[n];
	} else if (0 == strcmp("-disp-fn", argv[n])) {
	    n++;
	    if (n >= argc) {
		usage("Missing required argument for -disp-fn.");
		exit(0);
	    }
	    disp_font = argv[n];
	} else if (0 == strcmp("-rows", argv[n])) {
	    n++;
	    if (n >= argc) {
		usage("Missing required argument for -rows.");
		exit(0);
	    }
	    if (1 != sscanf(argv[n], "%d", &rows)) {
		usage("Unable to read number or rows.");
		exit(0);
	    }
	} else if (0 == strcmp("-cols", argv[n])) {
	    n++;
	    if (n >= argc) {
		usage("Missing required argument for -cols.");
		exit(0);
	    }
	    if (1 != sscanf(argv[n], "%d", &cols)) {
		usage("Unable to read number or cols.");
		exit(0);
	    }
	} else if (0 == strcmp("-b", argv[n])) {
	    drawButtons = 0;
	} else if (0 == strcmp("-bm", argv[n])) {
	    n++;
	    if (n >= argc) {
		usage("Missing required argument for -m.");
		exit(0);
	    }
	    if (0 == strcmp("dec", argv[n])) {
		setBaseMode(DECIMAL);
	    } else if (0 == strcmp("eng", argv[n])) {
		setBaseMode(DECIMAL_ENG);
	    } else if (0 == strcmp("bin", argv[n])) {
		setBaseMode(BINARY);
	    } else if (0 == strcmp("oct", argv[n])) {
		setBaseMode(OCTAL);
	    } else if (0 == strcmp("hex", argv[n])) {
		setBaseMode(HEXADECIMAL);
	    } else {
		usage("Specify dec, eng, bin, oct or hex for -m.");
		exit(0);
	    }
	} else {
	    usage("Unknown Argument.");
	    exit(0);
	}
	n++;
    }

    /* set up any constants we may use */
    setup_constant();

    /* initialize the undo system */
    initUndoSystem();

    /* setup the stack */
    if (0 == setup_stack()) {
	fprintf(stderr, "Error: Could not initalize data structures.\n");
	exit(0);
    }

#ifdef USE_GNOME
    main_w = gnome_app_new("grpn", "grpn");
    setup_menu(main_w);

    appbar = GNOME_APPBAR(gnome_appbar_new(FALSE, FALSE, FALSE));
    gnome_app_set_statusbar(GNOME_APP(main_w), GTK_WIDGET(appbar));

    /*create the box that everyone goes in */
    vbox = gtk_vbox_new(FALSE, 0);
    gnome_app_set_contents(GNOME_APP(main_w), vbox);
    gtk_widget_show(vbox);

#else
    /* the main window contains the work area and the menubar */
    main_w = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_widget_set_name(main_w, "grpn");

    /* create the box that everyone goes in */
    vbox = gtk_vbox_new(FALSE, 0);
    gtk_container_add(GTK_CONTAINER(main_w), vbox);
    gtk_widget_show(vbox);

    /* set up the menu bar */
    setup_menu(vbox);
#endif

    /* handle window manager close */
    gtk_signal_connect(GTK_OBJECT(main_w), "delete_event",
		       GTK_SIGNAL_FUNC(delete_event), NULL);
    gtk_signal_connect(GTK_OBJECT(main_w), "destroy",
		       GTK_SIGNAL_FUNC(destroy), NULL);

    /* create the varrious subsystems */
    setupModeDisplay(vbox);
    if (drawButtons)
	setupButtons(vbox, btn_font);
    setupLCD(vbox, rows, cols, disp_font);

    gtk_widget_show(main_w);

    icon_bitmap = gdk_bitmap_create_from_data(main_w->window,
					      (char *)grpn_bits,
					      grpn_width,
					      grpn_height);
    gdk_window_set_icon(main_w->window, NULL, icon_bitmap, NULL);

    gtk_main();

    return 0;
}
