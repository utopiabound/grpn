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
# add -DUSE_GNOME if you want to make grpn GNOME3 compliant.
CFLAG_OPTIONS = 

CFLAGS = -Wall -g -O2 $(shell pkg-config --cflags x11 gtk+-2.0)
#CFLAGS += -DUSE_GNOME `pkg-config --cflags libgnomeui-2.0`
CFLAGS += ${CFLAG_OPTIONS}
#-DGTK_DISABLE_DEPRECATED

LIBS = $(shell pkg-config --libs x11 gtk+-2.0) -lm -lgmp
#LIBS += `pkg-config --libs libgnomeui-2.0`

# end of user configurable section

OBJS = real.o complex.o matrix.o number.o \
        funcs.o constant.o button_def.o run_cmd.o \
	stack.o lcd.o setup_menu.o callback_menu.o \
	error.o process_input.o options.o \
	setup_buttons.o callback_buttons.o editor.o \
	help.o mode.o undo.o

grpn: $(OBJS) main.o
	$(CC) $(DFLAGS) -o $@ $^ $(LIBS)

test:: testor
	./testor

testor: $(OBJS) test.o
	$(CC) -lm -lgmp -o $@ $? $(LIBS)

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
	$(CC) -MM ${CFLAG_OPTIONS} *.c >> .depend

TAGS:
	etags *.[ch]

clean::
	rm -f core* grpn *~ *.o TAGS testor

include .depend

