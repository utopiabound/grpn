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
/* run_cmd.c  by Paul Wilkins */

#include <stdio.h>
#include <string.h>
#include <gtk/gtk.h>

#include "buttons.h"
#include "funcs.h"
#include "editor.h"
#include "error.h"


/* line is the name of the command to run, as stored in
 * the RowInfo struct 
 */
void runCmd(char *line){
   int i, j;
   int cmdLen;
   int lineLen;
   FuncInfo *fi;
   void (*CallBack)(GtkWidget *, gpointer);
   int found = 0;  /* how many matching commands have we found */

   if(*line == '"' || *line == '\'') line++;

   lineLen = strlen(line);

   /* try to find a matching command */
   for(i=0; i<NumFunctionRows; i++){
      for(j=0; j<rowinf[i].numBtns; j++){
         if(rowinf[i].fi[j].cmd != NULL){

            cmdLen = strlen(rowinf[i].fi[j].cmd);

            if(lineLen <= cmdLen){

               if(0 == strncmp(line, rowinf[i].fi[j].cmd, lineLen)){

                  /* store the command */
                  fi = &(rowinf[i].fi[j]);
                  found++;

                  if(lineLen == cmdLen){

                     /* we know this is the command */
                     found = 1;
                     goto MATCH_DONE;
                  }
               }
            }
         }
      }
   }

MATCH_DONE:

   if(found == 1){
      /* doh- we get called recursively is we don't do this */
      cancelEditor();

      /* run the command */
      CallBack = fi->CB;
      CallBack((GtkWidget *)NULL, (gpointer)fi);

   } else if(found > 1){
      sprintf(getStringError(), "%s: Ambigious command name.", line);

   } else {
      sprintf(getStringError(), "%s: Unrecognized command.", line);
   }
}


