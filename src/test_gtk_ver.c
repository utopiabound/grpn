/*

Copyright (C)  2000 Paul Wilkins

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See theGNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

*/
/* test_gtk_ver.c  by Paul Wilkins 1/30/99 */

#include <gtk/gtk.h>

/*
 * This will test to make sure you have the correct #define set
 * in the makefile for your version of GTK.
 *
 * if you get an error when compiling this file, then uncomment
 * the line #define GTK_VER_1_1 in the Makefile, and recompile.
 *
 * GTK version == 1.0   unset GTK_VER_1_1
 * GTK version == 1.1   set GTK_VER_1_1
 * GTK version == 1.2   set GTK_VER_1_1
 */

#ifdef GTK_VER_1_1  

typedef GtkTargetEntry WRONG_VERSION_OF_GTK_PLEASE_UNSET_GTK_VER_1_1_IN_MAKEFILE;

#endif
