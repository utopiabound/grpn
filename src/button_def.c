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
/*button_def.c  by Paul Wilkins 3/30/97 */

#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>

#include "buttons.h"
#include "editor.h"
#include "lcd.h"
#include "funcs.h"
#include "mode.h"
#include "undo.h"


extern void baseCmdCB(GtkWidget *, gpointer);
extern void cmodeCmdCB(GtkWidget *, gpointer);
extern void radixCmdCB(GtkWidget *, gpointer);

FuncInfo invisible1[] = {
   { "Modulo", "mod", "Remainder of division.",
      genericButtonCB, (void *)ModStack },
   { "Hex", "hex", "Hexadecimal display mode.",
      baseCmdCB, (void *)HEXADECIMAL },
   { "Dec", "dec", "Decimal display mode.",
      baseCmdCB, (void *)DECIMAL },
   { "Eng", "eng", "ENG Decimal display mode.",
      baseCmdCB, (void *)DECIMAL_ENG },
   { "Oct", "oct", "Octal display mode.",
      baseCmdCB, (void *)OCTAL },
   { "Bin", "bin", "Binary display mode.",
      baseCmdCB, (void *)BINARY },
   { "Rect", "rect", "Rectangular display mode.",
      cmodeCmdCB, (void *)RECTANGULAR },
   { "Polar", "pol", "Polar display mode.",
      cmodeCmdCB, (void *)POLAR },
   { "Deg", "deg", "Angles in degree.",
      radixCmdCB, (void *)DEGREES },
   { "Rad", "rad", "Angles in radian.",
      radixCmdCB, (void *)RADIANS },
   { "Db10", "db10", "Convert to dB (Power).",
      genericButtonCB, (void *)Db10Stack },
   { "Db20", "db20", "Convert to dB (Voltage).",
      genericButtonCB, (void *)Db20Stack },
   { "Rip", "rip", "Resistors in parallel.",
      genericButtonCB, (void *)RipStack },
   { "Cplx", "cplx", "Create or decompose a complex number.", 
      genericButtonCB, (void *)CplxStack },
   { "Cplx", "complex", "Create or decompose a complex number.",
      genericButtonCB, (void *)CplxStack },
   { "Mtrx", "mtrx", "Create or decompose a matrix.",
      genericButtonCB, (void *)MtrxStack },
   { "Mtrx", "matrix", "Create or decompose a matrix.", 
      genericButtonCB, (void *)MtrxStack },
   { "LShift", "<<", "Left Shift Stack",
     genericButtonCB, (void *)LShiftStack },
   { "LShift", "lshift", "Left Shift Stack",
     genericButtonCB, (void *)LShiftStack },
   { "RShift", ">>", "Right Shift Stack",
     genericButtonCB, (void *)RShiftStack },
   { "RShift", "rshift", "Right Shift Stack",
     genericButtonCB, (void *)RShiftStack },
   { "And", "and", "Bitwise AND",
     genericButtonCB, (void *)BitwiseANDStack },
   { "And", "&", "Bitwise AND",
     genericButtonCB, (void *)BitwiseANDStack },
   { "Or", "or", "Bitwise OR",
     genericButtonCB, (void *)BitwiseORStack },
   { "Or", "|", "Bitwise OR",
     genericButtonCB, (void *)BitwiseORStack },
   { "Xor", "xor", "Bitwise XOR",
     genericButtonCB, (void *)BitwiseXORStack },
   { "E", "e", "The constant E.",
     genericButtonCB, (void *)EStack },
   { "I", "i", "The constant i.",
     genericButtonCB, (void *)IStack },
   { "Undo", "undo", "Undo last command.  Up to 10 commands can be undone.", 
      genericButtonCB, (void *)UndoStack },
   { "Clear", "clear", "Clears and removes all numbers on the stack.",
      genericButtonCB, (void *)clearLCD },
   { "Quit", "quit", "Quit GRPN.", genericButtonCB, (void *)exit }
};
FuncInfo row2[] = {
   { "Enter", "dup", "Copy the number on the top of the stack.",
      genericButtonCB, (void *)PushStack },
   { "+/-", "neg", "Change sign.", plusMinusCB, NULL },
   { "EEX", NULL, "Mouse input: enter an exponent.",
      enterNumCB, (void *)'e' },
   { "DEL", NULL, "Mouse input: backspace.",
      genericButtonCB, (void *)deleteEditor },
   { "Drop", "drop", "Delete the number on the top of the stack.",
      genericButtonCB, (void *)PopStack },
   { "Swap", "swap", "Swap 2 numbers on the top of the stack.",
      genericButtonCB, (void *)SwapStack }
};
FuncInfo num789[] = {
   { "7", NULL, NULL, enterNumCB, (void *)'7' },
   { "8", NULL, NULL, enterNumCB, (void *)'8' },
   { "9", NULL, NULL, enterNumCB, (void *)'9' },
   { "/", NULL, NULL, genericButtonCB, (void *)DivStack },
   { "Pi", "pi", "The constant PI.", genericButtonCB, (void *)PiStack },
   { "Sin", "sin", "Trigonometric function Sin.",
      genericButtonCB, (void *)SinStack },
   { "Cos", "cos", "Trigonometric function Cos.",
      genericButtonCB, (void *)CosStack },
   { "Tan", "tan", "Trigonometric function Tan.",
      genericButtonCB, (void *)TanStack }
};
FuncInfo num456[] = {
   { "4", NULL, NULL, enterNumCB, (void *)'4' },
   { "5", NULL, NULL, enterNumCB, (void *)'5' },
   { "6", NULL, NULL, enterNumCB, (void *)'6' },
   { "x", NULL, NULL, genericButtonCB, (void *)MulStack },
   { "1/x", "inv", "Inverse.", genericButtonCB, (void *)InvStack },
   { "Asin", "asin", "Trigonometric function Arc-sin.",
      genericButtonCB, (void *)AsinStack },
   { "Acos", "acos", "Trigonometric function Arc-cos.",
      genericButtonCB, (void *)AcosStack },
   { "Atan", "atan", "Trigonometric function Arc-tan.",
      genericButtonCB, (void *)AtanStack }
};
FuncInfo num123[] = {
   { "1", NULL, NULL, enterNumCB, (void *)'1' },
   { "2", NULL, NULL, enterNumCB, (void *)'2' },
   { "3", NULL, NULL, enterNumCB, (void *)'3' },
   { "-", NULL, NULL, genericButtonCB, (void *)SubStack },
   { "Sqrt", "sqrt", "Square root.",
      genericButtonCB, (void *)SqrtStack },
   { "Nroot", "nroot", "N-th root.",
      genericButtonCB, (void *)NrootStack },
   { "Log", "log", "Log base 10.",
      genericButtonCB, (void *)LogStack },
   { "ln", "ln", "Natural log (log base e).",
      genericButtonCB, (void *)LnStack }
};
FuncInfo num0[] = {
   { "0", NULL, NULL, enterNumCB, (void *)'0' },
   { ".", NULL, NULL, enterNumCB, (void *)'.' },
   { NULL, NULL, NULL, NULL, NULL },
   { "+", NULL, NULL, genericButtonCB, (void *)AddStack },
   { "  2\nx  ", "sqr", "Square.", genericButtonCB, (void *)SqrStack },
   { "  x\ny  ", "pow", "Power.", genericButtonCB, (void *)PowStack },
   { "  x\n10 ", "tenx", "Ten to the power.",
      genericButtonCB, (void *)TenxStack },
   { "  x\ne  ", "exp", "e to the power.",
      genericButtonCB, (void *)ExpStack }
};

int NumButtonRows = 5;
int NumFunctionRows = 6;

struct RowInfo rowinf[] = {
   { 6, 6, row2 },
   { 8, 8, num789 },
   { 8, 8, num456 },
   { 8, 8, num123 },
   { 8, 8, num0 },
   { sizeof(invisible1)/sizeof(*invisible1), 0, invisible1 }
};


