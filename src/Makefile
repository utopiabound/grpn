# Makefile by Paul Wilkins
# this will compile the program 'grpn'
# Please read the next few lines and enter the correct values.

# where to install GRPN
PREFIX = /usr/local
MANDIR = ${PREFIX}/man

# What compiler should be used
CC = gcc 

# NOTE:
#
# uncomment -DUSE_GNOME if you want to make grpn GNOME compliant.
# uncomment -DUSE_PANGO if you want to use pango fonts (via gtk2)


CFLAGS = -Wall -g -O2 $(shell pkg-config --cflags x11 gtk+-2.0)
#CFLAGS += -DUSE_GNOME `pkg-config --cflags libgnomeui-2.0`
CFLAGS +=  -DUSE_PANGO

LIBS = $(shell pkg-config --libs x11 gtk+-2.0) -lm -lgmp
#LIBS += `pkg-config --libs libgnomeui-2.0`
#LIBS +=  `pkg-config --libs pango`

# end of user configurable section



OBJS = real.o complex.o matrix.o number.o \
        funcs.o constant.o button_def.o run_cmd.o \
	main.o stack.o lcd.o setup_menu.o callback_menu.o \
	process_input.o options.o \
	setup_buttons.o callback_buttons.o editor.o \
	error.o help.o mode.o undo.o

grpn:	$(OBJS)
	$(CC) $(DFLAGS) -o $@ $(OBJS) $(LIBS) 

pure:	$(OBJS)
	$(PURIFY) $(CC) $(DFLAGS) -o $@ $(OBJS) $(LIBS) 

install: grpn
	mkdir -p $(DESTDIR)$(PREFIX)/bin
	cp grpn $(DESTDIR)$(PREFIX)/bin
	chmod 755 $(DESTDIR)$(PREFIX)/bin/grpn
	mkdir -p $(DESTDIR)$(MANDIR)/man1
	cp grpn.1 $(DESTDIR)$(MANDIR)/man1
	chmod 644 $(DESTDIR)$(MANDIR)/man1/grpn.1

.c.o:
	$(CC) -c -o $@ $(CFLAGS) $*.c

depend::
	rm -f .depend
	$(CC) -MM $(CFLAGS) *.c >> .depend

TAGS:
	etags *.[ch]

clean::
	rm -f core* grpn *~ *.o TAGS

include .depend

