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


typedef struct _A A;
typedef struct _B B;

struct _A {
   int a;
   B *b;   
};



struct _B {
   int b;
   A *a;
};


main(){
   A a;
   B b;
   
   a.a = 5;
   b.b = 9;
   a.b = &b;
   b.a = &a;
   
   printf("a.a = %d\n", a.a);
   printf("b.b = %d\n", b.b);

   printf("a.b->b = %d\n", a.b->b);
   printf("b.a->a = %d\n", b.a->a);
}

