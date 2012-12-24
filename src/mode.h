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
/* mode.h  by Paul Wilkins 2/8/98 */

#ifndef __MODE_H
#define __MODE_H

#include <gtk/gtk.h>

#define BINARY      0x1
#define OCTAL       0x2
#define DECIMAL     0x4
#define HEXIDECIMAL 0x8

#define DEGREES 0x10
#define RADIANS 0x20

#define RECTANGULAR 0x40
#define POLAR 0x80

#define LONG_DISPLAY 0x100
#define SHORT_DISPLAY 0x200


void setPolarMode(int);
int getPolarMode();

void setRadixMode(int);
int getRadixMode();

void setBaseMode(int);
int getBaseMode();


/* the mode display window */
void refreshModeDisplay();

GtkWidget *setupModeDisplay(GtkWidget *);

#endif

