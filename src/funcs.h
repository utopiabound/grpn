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
/* funcs.h  by Paul Willkins 3/20/97 */

#ifndef __FUNCS_H
#define __FUNCS_H

#include <gtk/gtk.h>

struct _FuncInfo {
    char *name;
    char *cmd;
    char *help;
    void (*CB)(GtkWidget *, gpointer);
    void *data;
};
typedef struct _FuncInfo FuncInfo;


/* functions to manipulate the stack */

void readLine(char *);
void DupStack();
void PopStack();
void PushStack();

void EStack();
void IStack();
void PiStack();

void NegStack();
void InvStack();

void LogStack();
void TenxStack();

void ExpStack();
void LnStack();

void PowStack();
void NrootStack();

void RipStack();
void Db10Stack();
void Db20Stack();

void SqrStack();
void SqrtStack();

void SinStack();
void CosStack();
void TanStack();
void AsinStack();
void AcosStack();
void AtanStack();

void AddStack();
void SubStack();
void MulStack();
void DivStack();
void ModStack();

void BitwiseANDStack();
void BitwiseORStack();
void BitwiseXORStack();

void SwapStack();
void CplxStack();
void MtrxStack();

void LShiftStack();
void RShiftStack();

#endif
