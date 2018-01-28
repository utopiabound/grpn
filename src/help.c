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
#include <string.h>
#include <gtk/gtk.h>

#include "help.h"
#include "funcs.h"
#include "buttons.h"
#include "license.h"
#include "version.h"


#define ABOUT_TXT \
"GRPN is a graphical reverse polish notation (RPN) calculator.\n\
\n\
By: Paul Wilkins\n\
    paul.wilkins at analog com\n\
    Fix_locale.dpatch by Wartan Hachaturow <wart at debian.org>\n\
    Add_includes.dpatch by Michael Bienia <geser at ubuntu.com>\n\
    gtk2.dpatch by Barry deFreese <bdefreese at debian.org>\n\
    Support for Pango-fonts by Jean-Pierre Demailly.\n\
    Shift, AND, OR and XOR operators by Nathaniel Clark.\n\
    Changes since version 1.1.2-3: Jens Getreu <getreu at web.de>\n\
\n\
"

#define HELP_TXT \
"\
GRPN is a graphical reverse polish notation (RPN) calculator.\n\
\n\
GRPN works with real numbers, complex numbers, matrices, and\n\
complex matrices.  Numbers can be displayed in 4 different\n\
radix modes, and complex numbers can be displayed in either\n\
Cartesian or polar form.\n\
\n\
GRPN uses a stack and reverse polish notation to evaluate\n\
expressions.  The stack in GRPN is infinite and limited only\n\
by available memory.\n\
\n\
Expressions are evaluated in reverse polish notation.  Unlike\n\
conventional (prefix) notation, RPN requires that all arguments\n\
to a command are entered prior to execution of the command.  For\n\
example to add 6.7 to 3.2 you would push both numbers onto the\n\
stack, then press the add (+) button:\n\
\n\
3.2<enter>\n\
6.7<enter>\n\
+\n\
\n\
The result is left on the stack.\n\
\n\
Cntr-c copies the result to the clipboard for use in \n\
other applications. \n\
\n\
Note that GRPN uses a shorthand for all commands that causes\n\
an automatic <enter> before executing a command.  The previous\n\
example could then be done as:\n\
\n\
3.2<enter>\n\
6.7+\n\
\n\
Input is accepted from both the keyboard and the mouse.  When\n\
entering a command from the keyboard, the command may be\n\
abbreviated to the shortest set of letters which uniquely\n\
identifies the command.\n\
\n\
Input may be forced to be interpreted as a command by prefixing\n\
the command with double or single quotes.  This is useful, for\n\
example when in Hexadecimal mode and you would like to switch to\n\
decimal mode by typing:\n\
dec\n\
\n\
Mouse input simply requires pressing the button with the desired\n\
command.\n\
\n\
To enter a matrix first push its data on the stack as you would\n\
lines, f. ex.:\n\
1<enter> 2<enter> 3<enter> 4<enter> 5<enter> 6<enter>\n\
\n\
Then you can enter the dimension of the matrix f. ex.:\n\
3<enter> 2<enter> matrix<enter>\n\
\n\
As you can see in the following output, the command 'matrix'\n\
creates (or decomposes) the matrix:\n\
1:  [1 2 3 \n\
     4 5 6]\n\
\n\
Complex numbers are entered in a similar way:\n\
3<enter> 7<enter> complex<enter>\n\
1:  (3,7)\n\
\n\
Note that the complex-command also decomposes\n\
complex numbers:\n\
complex<enter>\n\
2:  3\n\
1:  7\n\
\n\
The 'undo' command allows you to undo up to the last 10\n\
operations.\n\
\n\
Available commands:\n\
\n\
+           Add.\n\
-           Subtract.\n\
*           Multiply.\n\
/           Divide.\n\
^           Power.\n\
<enter>     Push a number onto the top of the stack.\n\
<space>     Push a number onto the top of the stack.\n\
"

void popup_window(GtkWidget **dialog, char *txt, char *title){


   GtkWidget *vbox;
   GtkWidget *scrolled_win;
   GtkWidget *label;
   GtkWidget *button;
#ifdef USE_PANGO
   PangoFontDescription *pango_desc;
#endif

 
   if (!*dialog) {

      *dialog = gtk_window_new(GTK_WINDOW_TOPLEVEL);
      gtk_window_set_title(GTK_WINDOW(*dialog), title);

      gtk_signal_connect(GTK_OBJECT(*dialog), "destroy",
                        GTK_SIGNAL_FUNC(gtk_widget_destroyed),
                        dialog);
#ifdef GTK_VER_1_1
      gtk_container_set_border_width(GTK_CONTAINER(*dialog), 5); 
#else
      gtk_container_border_width(GTK_CONTAINER(*dialog), 5); 
#endif

      gtk_window_set_title(GTK_WINDOW(*dialog), title);
      gtk_widget_set_usize(*dialog, 470, 470);

      vbox = gtk_vbox_new(FALSE, 0);
      gtk_container_add(GTK_CONTAINER(*dialog), vbox);
      gtk_widget_show(vbox);

      scrolled_win = gtk_scrolled_window_new (NULL, NULL);
#ifdef GTK_VER_1_1
      gtk_container_set_border_width(GTK_CONTAINER(scrolled_win), 5); 
#else
      gtk_container_border_width(GTK_CONTAINER(scrolled_win), 5); 
#endif
      gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW (scrolled_win),
                                     GTK_POLICY_AUTOMATIC,
                                     GTK_POLICY_AUTOMATIC);
      gtk_box_pack_start(GTK_BOX(vbox), scrolled_win, TRUE, TRUE, 0);
      gtk_widget_show(scrolled_win);

      label = gtk_label_new(txt);
      gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_LEFT);
#ifdef GTK_VER_1_1
      gtk_scrolled_window_add_with_viewport(GTK_SCROLLED_WINDOW(scrolled_win), label);
#else
      gtk_container_add(GTK_CONTAINER(scrolled_win), label);
#endif
#ifdef USE_PANGO
      // Pick the default Monospaced font
      pango_desc = pango_font_description_from_string("Mono");
      if (pango_desc)
	  gtk_widget_modify_font(label, pango_desc);
#endif
      gtk_widget_show(label);

      button = gtk_button_new_with_label("Dismiss");
      gtk_signal_connect_object(GTK_OBJECT(button), "clicked",
                                 GTK_SIGNAL_FUNC(gtk_widget_destroy),
                                 GTK_OBJECT(*dialog));
      gtk_box_pack_end(GTK_BOX(vbox), button, FALSE, FALSE, 0);
      GTK_WIDGET_SET_FLAGS(button, GTK_CAN_DEFAULT);
      /* gtk_widget_grab_default(button);  This puts an ugly box around the botton */
      gtk_widget_show(button);
 
   }

   if (!GTK_WIDGET_VISIBLE(*dialog))
      gtk_widget_show(*dialog);
   else
      gtk_widget_destroy(*dialog);

}

void license_popup(){
   int i;
   char *htxt;
   static GtkWidget *licenseDialog = NULL;

   if(NULL == (htxt = (char*)malloc((10000)*sizeof(char)))){
      perror("license_popup: malloc");
      return;
   } else {
      strcpy(htxt, LICENSE_TXT);
      //popup_window wants a long text to scroll
      for(i=0; i<60; i++){
	       strcat(htxt, "\n");
      }
   }
   popup_window(&licenseDialog, htxt, "License");
   free(htxt);
}

void help_popup(){
   int i, j, k;
   int cmds;
   int len;
   char *htxt;
   char *cmd;
   static GtkWidget *helpDialog = NULL;

   cmds = 0;
   for(i=0; i<NumFunctionRows; i++){
      cmds += rowinf[i].numBtns;
   }
   if(NULL == (htxt = (char*)malloc((10000+cmds*60)*sizeof(char)))){
      perror("help_popup: malloc");
      return;
   } else {
      strcpy(htxt, HELP_TXT);
      // append the list of commands to the help text
      for(i=0; i<NumFunctionRows; i++){
	 for(j=0; j<rowinf[i].numBtns; j++){
	    cmd = rowinf[i].fi[j].cmd;
	    if(cmd != NULL){
	       strcat(htxt, cmd);
	       len = 12 - strlen(cmd);
	       for(k=0; k<len; k++) strcat(htxt, " ");
	       strcat(htxt, rowinf[i].fi[j].help);
	       strcat(htxt, "\n");
	    }
	 }
      }
   }

   popup_window(&helpDialog, htxt, "Help");

   free(htxt);

}

void about_popup(){
   int i;
   char *htxt;
   static GtkWidget *aboutDialog = NULL;
 
   if(NULL == (htxt = (char*)malloc((10000)*sizeof(char)))){
      perror("about_popup: malloc");
      return;
   } else {
      strcpy(htxt, GRPN_VERSION);
      strcat(htxt, ABOUT_TXT);
      //popup_window wants a long text to scroll
      for(i=0; i<60; i++){
	       strcat(htxt, "\n");
      }
   }
   popup_window(&aboutDialog, htxt, "About");
   free(htxt);
}

