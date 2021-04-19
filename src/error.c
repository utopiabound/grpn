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
/* error.c  by Paul Wilkins 3/30/97 */

#include <stdio.h>
#include <string.h>

#include "error.h"
#include "stack.h"

char ErrorString[MAX_ERROR_SIZE] = { 0 };

void resetError() {
    *ErrorString = '\0';
}

int isError() {
    return *ErrorString != '\0';
}

char *getStringError() {
    return ErrorString;
}

void setStringError(char *error) {
    strncpy(ErrorString, error, MAX_ERROR_SIZE);
    ErrorString[MAX_ERROR_SIZE-1] = '\0';
}


int checkArgs(char *name, int args) {
    if (stackLen() < args) {
	sprintf(ErrorString, "%s Error: Too few Arguments.", name);
	return 1;
    }
    return 0;
}

void invalidTyprError(char *name) {
    sprintf(ErrorString, "%s Error: Invalid Type.", name);
}
