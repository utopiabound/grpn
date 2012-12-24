# Makefile by Paul Wilkins
# this will compile the program 'grpn'
# Please read the next few lines and enter the correct values.

# where to install GRPN
PREFIX = /usr/local/

# What compiler should be used
CC = gcc 

# Where to find the gtk-config script
GTK_DIR =
#GTK_DIR = /loc/libs/gtk+-1.0.6/bin/


# NOTE:
#
# add -DGTK_VER_1_1 to the CFLAGS if you are using GTK version 1.1.0 or higher
#
# add -DUSE_GNOME if you want to make grpn GNOME compliant.


CFLAGS = -g -O2 -I/usr/X11/include  `$(GTK_DIR)gtk-config --cflags` -DGTK_VER_1_1

DFLAGS =  -L/usr/X11/lib 

# end of user configurable section



OBJS = test_gtk_ver.o real.o complex.o matrix.o number.o \
        funcs.o constant.o button_def.o run_cmd.o \
	main.o stack.o lcd.o setup_menu.o callback_menu.o \
	process_input.o options.o \
	setup_buttons.o callback_buttons.o editor.o \
	error.o help.o mode.o undo.o



LIBS   = `$(GTK_DIR)gtk-config --libs` -lX11 -lm 


grpn:	$(OBJS)
	$(CC) $(DFLAGS) -o $@ $(OBJS) $(LIBS) 

pure:	$(OBJS)
	$(PURIFY) $(CC) $(DFLAGS) -o $@ $(OBJS) $(LIBS) 

install:	grpn
	mkdirhier $(PREFIX)/bin
	cp grpn $(PREFIX)/bin
	chmod 755 $(PREFIX)/bin/grpn
	mkdirhier $(PREFIX)/man/man1
	cp grpn.1 $(PREFIX)/man/man1
	chmod 644 $(PREFIX)/man/man1/grpn.1

.c.o:
	$(CC) -c -o $@ $(CFLAGS) $*.c

depend:
	rm -f .depend
	$(CC) -MM $(CFLAGS) *.c >> .depend


clean:
	rm -f core grpn *.o

include .depend

