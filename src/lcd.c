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
/* lcd.c   by Paul Wilkins 3/22/97 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gtk/gtk.h>
#include <gdk/gdk.h>
#include <gdk/gdkkeysyms.h>

#include "lcd.h"
#include "mode.h"
#include "number.h"
#include "stack.h"
#include "process_input.h"
#include "editor.h"
#include "error.h"
#include "undo.h"

/* the amount of spacing we want around the display */
#define BORDER 2 

GtkWidget *lcdDA = NULL;

char **lcdText = NULL;
GdkGC *lcdGC = 0;
GdkGC *lcdHighlightGC = 0;

/* the last area highlighted */
int dataSelected = 0;
int hiX0, hiX1, hiY1;
int hiY0, hiX2, hiY2;

/* font stuff */
#ifdef USE_PANGO
   PangoLayout *pango_layout;
   PangoFontDescription *pango_desc;
   PangoFontMetrics *pango_metrics;
   PangoFontMap *pango_fontmap;
   PangoContext *pango_context;
   PangoFont *pango_font;
   PangoFontMetrics *pango_metrics;
   #define SHIFT 3
#else
   GdkFont *lcdFont;
   #define SHIFT -1
   #define FONT1 "12x24"
   #define FONT2 "-*-courier-medium-r-*-*-24-*"
   #define FONT3 "-misc-fixed-medium-r-normal-*-20-*"
   #define FONT4 "10x20"
   #define FONT5 "-*-courier-medium-r-*-*-12-*"
   #define FONT6 "fixed"
#endif

int fontW;
int fontH;
int fontD;

int lcdWidth = 0;
int lcdHeight = 0;
int lcdDisplayMode = LONG_DISPLAY;

int lcdInitialized = 0;

/* some callbacks */
static gint lcdExposeCB(GtkWidget *widget, GdkEventExpose *event);
static gint lcdResizeCB(GtkWidget *widget, GdkEventConfigure *event);

/* local functions */
void calcStackLCD();
void drawStackLCD();

/* this stuff for cut & paste */
#define START_SELECTING 0x1
#define NONE_SELECTED 0x2
#define DATA_SELECTED 0x4

static gint lcdButtonPressEvnt(GtkWidget *widget, GdkEventButton *event);
static gint lcdButtonReleaseEvnt(GtkWidget *widget, GdkEventButton *event);
static gint lcdMotionEvnt(GtkWidget *widget, GdkEventMotion *event);
static gint lcdKeyPressEvnt(GtkWidget *widget, GdkEventKey *event);
static gint lcdKeyReleaseEvnt(GtkWidget *widget, GdkEventKey *event);

void lcdPasteCB(GtkWidget *, GtkSelectionData *, gpointer);
gint loseSelection(GtkWidget *, GdkEventSelection *);
#ifdef GTK_VER_1_1
void convertSelection(GtkWidget *, GtkSelectionData *,
   guint, guint, gpointer);
#else
void convertSelection(GtkWidget *, GtkSelectionData *,
   gpointer);
#endif

void lcdGetSelection(int time);
void lcdStartHighlight(int x, int y, int time);
void lcdStopHighlight(int x, int y, int time);
void lcdContinueHighlight(int x, int y);


#define TARGET_STRING 0x1
#define TARGET_TEXT 0x2
#define TARGET_COMPOUND_TEXT 0x4

GtkWidget *setupLCD(GtkWidget *parent, int rows, int cols, char *font){
   int wid, hgt;
#ifdef GTK_VER_1_1
   static GtkTargetEntry targetlist[] = {
     /* Target          Flags  Info      */
     { "STRING",        0,     TARGET_STRING },
     { "TEXT",          0,     TARGET_TEXT },
     { "COMPOUND_TEXT", 0,     TARGET_COMPOUND_TEXT } 
   };
   static gint ntargets = sizeof(targetlist) / sizeof(targetlist[0]);
#endif

   /* store arguments */
   lcdWidth = cols;
   lcdHeight = rows;

#ifdef USE_PANGO
   lcdDA = gtk_drawing_area_new();
   pango_layout = gtk_widget_create_pango_layout(lcdDA, NULL);
   pango_desc = pango_font_description_from_string(font?font:"Liberation Mono 16");
   pango_fontmap = pango_cairo_font_map_get_default();
   pango_context = pango_font_map_create_context(pango_fontmap);
   pango_font = pango_context_load_font(pango_context, pango_desc);
   pango_metrics =
      pango_context_get_metrics(pango_context, pango_desc, pango_language_get_default());

   fontW = (pango_font_metrics_get_approximate_digit_width(pango_metrics))/PANGO_SCALE;
   fontH = (pango_font_metrics_get_ascent(pango_metrics) + pango_font_metrics_get_descent(pango_metrics))/PANGO_SCALE;
   fontD = pango_font_metrics_get_descent(pango_metrics)/PANGO_SCALE;

   gtk_widget_modify_font(lcdDA, pango_desc);
#else
   fontW = 0;
   fontH = 0;
   fontD = 0;

   /* get a font for the main window */
   if(font != NULL){
      if(NULL == (lcdFont = gdk_font_load(font))){
         fprintf(stderr, "Unable to load font %s.\n", font);
         exit(0);
      }
   } else if(NULL == (lcdFont = gdk_font_load(FONT1)) &&
      NULL == (lcdFont = gdk_font_load(FONT2)) &&
      NULL == (lcdFont = gdk_font_load(FONT3)) &&
      NULL == (lcdFont = gdk_font_load(FONT4)) &&
      NULL == (lcdFont = gdk_font_load(FONT5)) &&
      NULL == (lcdFont = gdk_font_load(FONT6))){

      fprintf(stderr, "Unable to load a font.\n");
      exit(0);
   }

   /* find max font width */
   for(int i=0; i<256; i++){
       int width = gdk_char_width(lcdFont, (gchar)i);
       if (width < 50 && width > fontW) fontW = width;
   }

   /* globals we use all over the place */
   fontH = lcdFont->ascent + lcdFont->descent;
   fontD = lcdFont->descent;
#endif

   if(fontW == 0 || fontH == 0){
      fprintf(stderr, "Error: can not determine font dimentions.\n");
      exit(0);
   }

   wid = (2 * BORDER) + (lcdWidth * fontW);
   hgt = (2 * BORDER) + (lcdHeight * fontH);

#ifndef USE_PANGO
   lcdDA = gtk_drawing_area_new();
#endif

   gtk_drawing_area_size(GTK_DRAWING_AREA(lcdDA), wid, hgt);
   gtk_box_pack_start(GTK_BOX(parent), lcdDA, TRUE, TRUE, 0);

   /* Signals used to handle window ops */
   gtk_signal_connect(GTK_OBJECT(lcdDA), "expose_event",
		     (GtkSignalFunc)lcdExposeCB, NULL);
   gtk_signal_connect(GTK_OBJECT(lcdDA),"configure_event",
		     (GtkSignalFunc)lcdResizeCB, NULL);

   /* Event signals (Selection) */
   gtk_signal_connect(GTK_OBJECT(lcdDA), "selection_received",
                     (GtkSignalFunc)lcdPasteCB, NULL);
   gtk_signal_connect(GTK_OBJECT(lcdDA), "selection_clear_event",
                     (GtkSignalFunc)loseSelection, NULL);
   /*
   gtk_signal_connect(GTK_OBJECT(lcdDA), "selection_request_event",
                     (GtkSignalFunc)convertSelection, NULL);
   */
#ifdef GTK_VER_1_1
   gtk_selection_add_targets(lcdDA,
      GDK_SELECTION_PRIMARY, targetlist, ntargets);

   gtk_signal_connect(GTK_OBJECT(lcdDA), "selection_get",
      (GtkSignalFunc)convertSelection, NULL);
#else
   gtk_selection_add_handler(lcdDA, GDK_SELECTION_PRIMARY,
      GDK_SELECTION_TYPE_STRING, convertSelection, NULL);
#endif


   /* Event signals (Input) */
   gtk_signal_connect(GTK_OBJECT(lcdDA), "motion_notify_event",
		     (GtkSignalFunc)lcdMotionEvnt, NULL);
   gtk_signal_connect(GTK_OBJECT(lcdDA), "button_press_event",
		     (GtkSignalFunc)lcdButtonPressEvnt, NULL);
   gtk_signal_connect(GTK_OBJECT(lcdDA), "button_release_event",
		     (GtkSignalFunc)lcdButtonReleaseEvnt, NULL);
   gtk_signal_connect_after(GTK_OBJECT(lcdDA), "key_press_event",
		     (GtkSignalFunc)lcdKeyPressEvnt, NULL);
   gtk_signal_connect_after(GTK_OBJECT(lcdDA), "key_release_event",
		     (GtkSignalFunc)lcdKeyReleaseEvnt, NULL);

   gtk_widget_set_events(lcdDA, 
      GDK_EXPOSURE_MASK | GDK_LEAVE_NOTIFY_MASK |
      GDK_BUTTON_PRESS_MASK | GDK_BUTTON_RELEASE_MASK |
      GDK_KEY_PRESS_MASK | GDK_KEY_RELEASE_MASK |
      GDK_POINTER_MOTION_MASK | GDK_POINTER_MOTION_HINT_MASK);

   GTK_WIDGET_SET_FLAGS(lcdDA, GTK_CAN_FOCUS);
   gtk_widget_show(lcdDA);
   gtk_widget_grab_focus(lcdDA);


   return lcdDA;
}

void clearLCD(){

   /* clear the stack */
   clearStack();

   /* reset any error string */
   resetError();

   /* clear the entry buffer */
   cancelEditor();

   /* re-initialize (and clear) the undo system */
   initUndoSystem();

   /* redraw the window */
   calcStackLCD();
   drawStackLCD();
}

void clearLCDwindow(){
   gdk_window_clear(lcdDA->window);
}


/* Recalculate the contents of the lcd display. 
 * note: we don't redraw the lcd here, that is done by drawStackLCD().
 */
void calcStackLCD(){
   int i, j, pLen, curPos;
   size_t strLen;
   char *c, *p;
   char *txt;     /* the number */
   char label[16];  /* the stack number label */
   int height;
   int indx;
   int row;       /* the row to start print the current number */
   int txtPos;    /* which col do we start printing in */
   int lines;     /* the number of lines this number has */
   int top;       /* which row is the top for drawing the stack */
   int bottom;    /* which row is the current bottom for drawing the stack */
   int labelLen;  /* the stack number label length */
   Number *num;

   if(isEditingEditor()){
      height = lcdHeight - 1;
   } else {
      height = lcdHeight;
   }

   if(height <= 0) return;

   /* clear the old display */
   for(i=0; i<lcdHeight; i++){
      for(j=0; j<lcdWidth; j++){
         lcdText[i][j] = ' ';
      }
   }

   /* check for an error */
   if (isError()) {
      top = 1;
      c = getStringError();
      strLen = strnlen(c, lcdWidth);
      strncpy(lcdText[0], c, strLen);
   } else {
      top = 0;
   }
   bottom = height;

   /* print all of the numbers */
   for(indx=0; indx<stackLen(); indx++){

      /* get the number to print */
      if(NULL == (num = getStackEle(indx))) break;

      /* get the text of the number */
      if(indx == 0 || lcdDisplayMode == LONG_DISPLAY){
	 if(NULL == (txt = printNumber(num))) break;
      } else {
	 if(NULL == (txt = printNumberShort(num))) break;
      }
      
      /* how many lines is this number? */
      lines = 1;
      for(p=txt; *p!='\0'; p++) if(*p=='\n'){ lines++; *p='\0'; }

      /* find the starting line of the number */
      row = bottom - lines;
      if(row < top) row = top;

      /* print the stack number label */
      sprintf(label, "%d: ", indx+1);
      labelLen = strnlen(label, lcdWidth);
      strncpy(lcdText[row], label, labelLen);

      /* print the number */
      p = txt;         
      for(j=0, i=row; i<bottom && j<lines; i++, j++){
	 pLen = strlen(p);
	 txtPos = lcdWidth - pLen;
	 if(txtPos < labelLen) txtPos = labelLen;
         if(txtPos > lcdWidth) break;

	 strncpy(&(lcdText[i][txtPos]), p, lcdWidth-txtPos);
	 p = p + pLen + 1;
      }

      /* free the mem used to print the number */
      free(txt);

      bottom = bottom - j;

      if(bottom == top) break;

   }

   /* fill in the rest of the indexes */
   for(i=indx, j=bottom; j>top; i++, j--){
      sprintf(label, "%d: ", i+1);
      labelLen = strnlen(label, lcdWidth);
      strncpy(lcdText[j-1], label, labelLen);
   }

   if(isEditingEditor()){

      txt = getLineEditor();
      curPos = cursorPosEditor();

      if(curPos > lcdWidth){
         txt += (curPos - lcdWidth);
      }
      strLen = strnlen(txt, lcdWidth);
      strncpy(lcdText[lcdHeight-1], txt, strLen);
   }

}

/* Redraw the contents of the lcd display. 
 * note: start BORDER pixels down and to the right.
 * TODO: we could be smart and only redraw what has changed. 
 * This does this work correctly with varriable sized fonts.
 */
void drawStackLCD(){
   int i;
   int curPos;
   int strt = 0, stop;
   GdkGC *drawgc;
#ifdef USE_PANGO
   PangoRectangle rect;
   int pango_pos;
#endif

   /* draw the stack */
   for(i=0; i<lcdHeight; i++){

      /* draw the first unhighlighted section of the line */
      strt = 0;
      if(dataSelected == DATA_SELECTED && i>=hiY1 && i<=hiY2){
         stop = hiX1;
         if(stop > lcdWidth) stop = lcdWidth;
      } else {
         stop = lcdWidth;
      }
      if(stop-strt){
#ifdef USE_PANGO
         pango_layout_index_to_pos(pango_layout, strt, &rect);
         pango_pos = rect.x / PANGO_SCALE;
         gdk_window_clear_area(lcdDA->window, 
            pango_pos + BORDER, i*fontH + BORDER,
            2*fontW*(stop-strt), fontH);
         pango_layout_set_text(pango_layout,  &lcdText[i][strt], stop-strt);
         gdk_draw_layout(lcdDA->window, lcdGC,
            pango_pos + BORDER + SHIFT, i*fontH + BORDER,
	    pango_layout);
#else
         gdk_window_clear_area(lcdDA->window, 
            strt*fontW + BORDER, i*fontH + BORDER,
            fontW*(stop-strt), fontH);
         gdk_draw_text(lcdDA->window, lcdFont, lcdGC,
            strt*fontW + BORDER, (i+1)*fontH - fontD + BORDER,
            &lcdText[i][strt], stop-strt);
#endif
      }

      /* draw the highlighted section of the line */
      if(dataSelected == DATA_SELECTED && i>=hiY1 && i<=hiY2){
	 strt = hiX1;
         if(strt > lcdWidth) strt = lcdWidth;
	 stop = hiX2+1;
         if(stop > lcdWidth) stop = lcdWidth;
         if(stop-strt){            
#ifdef USE_PANGO
            pango_layout_index_to_pos(pango_layout, strt, &rect);
            pango_pos = rect.x / PANGO_SCALE;
            gdk_window_clear_area(lcdDA->window, 
               pango_pos + BORDER, i*fontH + BORDER,
               2*fontW*(stop-strt), fontH);
            gdk_draw_rectangle(lcdDA->window, lcdGC, TRUE,
               pango_pos + BORDER + SHIFT, i*fontH + BORDER,
               fontW*(stop-strt), fontH);
            pango_layout_set_text(pango_layout,  &lcdText[i][strt], stop-strt);
            gdk_draw_layout(lcdDA->window, lcdHighlightGC,
               strt*fontW + BORDER + SHIFT, i*fontH + BORDER,
	       pango_layout);
#else
            gdk_draw_rectangle(lcdDA->window, lcdGC, TRUE,
               strt*fontW + BORDER, i*fontH + BORDER,
               fontW*(stop-strt), fontH);
	    gdk_draw_text(lcdDA->window, lcdFont, lcdHighlightGC,
	       strt*fontW + BORDER, (i+1)*fontH - fontD + BORDER,
	       &lcdText[i][strt], stop-strt);
#endif
         }
      }

      /* draw the last unhighlighted section of the line */
      stop = lcdWidth;
      if(dataSelected == DATA_SELECTED && i>=hiY1 && i<=hiY2){
         strt = hiX2+1;
         if(strt > lcdWidth) strt = lcdWidth;
      } else {
         strt = lcdWidth;
      }
      if(stop-strt){
#ifdef USE_PANGO
         pango_layout_index_to_pos(pango_layout, strt, &rect);
         pango_pos = rect.x / PANGO_SCALE;
         gdk_window_clear_area(lcdDA->window, 
            pango_pos + BORDER, i*fontH + BORDER,
            2*fontW*(stop-strt), fontH);
         pango_layout_set_text(pango_layout,  &lcdText[i][strt], stop-strt);
         gdk_draw_layout(lcdDA->window, lcdGC,
            pango_pos + BORDER + SHIFT, i*fontH + BORDER,
	    pango_layout);
#else
	 gdk_window_clear_area(lcdDA->window, 
	    strt*fontW + BORDER, i*fontH + BORDER,
	    fontW*(stop-strt), fontH);
         gdk_draw_text(lcdDA->window, lcdFont, lcdGC,
            strt*fontW + BORDER, (i+1)*fontH - fontD + BORDER,
            &lcdText[i][strt], stop-strt);
#endif
      }
   }

   /* draw the cursor */
   if(isEditingEditor()){
      curPos = cursorPosEditor();
      if(curPos > lcdWidth){
         curPos = lcdWidth;
      }
      if(dataSelected == DATA_SELECTED &&
	 curPos>=hiX1 && curPos<=hiX2 && 
         lcdHeight-1>=hiY1 && lcdHeight-1<=hiY2)
      {
         drawgc = lcdHighlightGC;
      } else {
	 drawgc = lcdGC;
      }

      /* hack: draw a white line to erase any old cursor
         in position 0. XDrawImageString dosen't cover this
         up like it does with an old cursor in any other position */
      if(!(dataSelected == DATA_SELECTED && 
         0==hiX1 && lcdHeight-1>=hiY1 && lcdHeight-1<=hiY2))
      {
         gdk_draw_line(lcdDA->window, lcdHighlightGC,
            BORDER + SHIFT, (lcdHeight-1) * fontH + BORDER,
            BORDER + SHIFT, lcdHeight * fontH - 1 + BORDER);
      }

      /* draw the cursor */
#ifdef USE_PANGO
      pango_layout_index_to_pos(pango_layout, curPos, &rect);
      pango_pos = rect.x / PANGO_SCALE + SHIFT + BORDER;
      gdk_draw_line(lcdDA->window, drawgc,
         pango_pos, (lcdHeight-1) * fontH + BORDER,
         pango_pos, lcdHeight * fontH - 1 + BORDER);
      /* Flush GDK display, seems to be needed in certain environments */
      gdk_flush();
   } else {
      pango_layout_index_to_pos(pango_layout, strt, &rect);
      pango_pos = rect.x / PANGO_SCALE + SHIFT + BORDER;
      if (pango_pos > lcdDA->allocation.width - BORDER && lcdWidth > 6) {
	 lcdWidth -= 1;
         clearLCDwindow();
         calcStackLCD();
         drawStackLCD();
      }
   }
#else
      gdk_draw_line(lcdDA->window, drawgc,
         fontW * curPos + SHIFT + BORDER, (lcdHeight-1) * fontH + BORDER,
         fontW * curPos + SHIFT + BORDER, lcdHeight * fontH - 1 + BORDER);
   }
#endif
}



/* redraw what we need to */ 
void redrawLCD(){

   calcStackLCD();
   drawStackLCD();

}


void lcdResize(){
   int i;
   char *buf;
   int width, height;

   width = lcdDA->allocation.width;
   height = lcdDA->allocation.height;

   lcdWidth = (width - 2 * BORDER) / fontW;
   if(lcdWidth < 0) lcdWidth = 0;

   lcdHeight = (height - 2 * BORDER) / fontH;
   if(lcdHeight < 0) lcdHeight = 0;
 
   /* free the old mem */
   if(lcdText){
      free(lcdText[0]);
      free(lcdText);
   }

   /* get the new mem. note: add 1 bec size could otherwize be 0. */
   if(NULL == (lcdText=(char**)malloc((1+lcdHeight)*sizeof(char *)))){
      perror("malloc");
      exit(0);
   }
   if(NULL == (buf=(char*)malloc(1+lcdHeight*lcdWidth*sizeof(char)))){
      perror("malloc");
      exit(0);
   }
   lcdText[0] = buf;  /* in case lcdHeight == 0 */
   for(i=0; i<lcdHeight; i++){
      lcdText[i] = &buf[lcdWidth * i];
   }
 
   /* we don't get an expose if the window shrinks */

   clearLCDwindow();

   calcStackLCD();
   drawStackLCD();
}



void setLCDDispMode(int val){

   if(val == LONG_DISPLAY) lcdDisplayMode = LONG_DISPLAY;
   else if(val == SHORT_DISPLAY) lcdDisplayMode = SHORT_DISPLAY;
   else printf("setShortLCD invalid mode\n");

}

int getLCDDispMode(){
   return lcdDisplayMode;
}

static gint lcdExposeCB(GtkWidget *widget, GdkEventExpose *event){
   GtkStyle *style;

   if(lcdInitialized == 0){

      style = gtk_widget_get_style(widget);

      /* Set the default Graphics Contexts */
      /* highlight swaps foreground and background */
      lcdGC = style->fg_gc[GTK_STATE_NORMAL];
      lcdHighlightGC = style->bg_gc[GTK_STATE_NORMAL];

      lcdResize();
      lcdInitialized = 1;

   }

   clearLCDwindow();

   drawStackLCD();

   return FALSE;
}


static gint lcdResizeCB(GtkWidget *widget, GdkEventConfigure *event){

   if(lcdInitialized != 0) lcdResize();

   return TRUE;
}


static gint lcdKeyPressEvnt(GtkWidget *widget, GdkEventKey *event){
   char c;

   if(event->string) c = *(event->string);
   else c = 'd'; /* just pick something */

   processInput(
      event->keyval, 
      event->state & GDK_CONTROL_MASK ? 1 : 0, 
      c, 
      'd');

   return TRUE;
}
static gint lcdKeyReleaseEvnt(GtkWidget *widget, GdkEventKey *event){
   return TRUE;
}



static gint lcdButtonPressEvnt(GtkWidget *widget, GdkEventButton *event) {
   gtk_widget_grab_focus(lcdDA);
   if(event->button == 1){
      lcdStartHighlight(event->x, event->y, event->time);
   }
   return TRUE;
}

static gint lcdButtonReleaseEvnt(GtkWidget *widget, GdkEventButton *event) {
   switch(event->button){
      case 1:       /* stop highlighting an area */
	 lcdStopHighlight(event->x, event->y, event->time);
	 break;
      case 2:       /* paste data */
	 lcdGetSelection(event->time);
	 break;
      case 3:       /* might be used to continue highlighting */
	 if(dataSelected == DATA_SELECTED){
	    lcdContinueHighlight(event->x, event->y);
	    lcdStopHighlight(event->x, event->y, event->time);
	 }
	 break;
   }
   return TRUE; 
}


static gint lcdMotionEvnt(GtkWidget *widget, GdkEventMotion *event)
{
   int x, y;
   GdkModifierType state;

   if(event->is_hint){
      gdk_window_get_pointer(event->window, &x, &y, &state);
   } else {
      x = event->x;
      y = event->y;
      state = event->state;
   }

   if(state & GDK_BUTTON1_MASK){

      /* highlighting with button 1 */
      lcdContinueHighlight(x, y);

   } else if(state & GDK_BUTTON3_MASK &&
      dataSelected == DATA_SELECTED){

      /* continuing a highlight with button 3 */
      lcdContinueHighlight(x, y);
   }

   return TRUE;
}

#ifdef GTK_VER_1_1
void convertSelection(
   GtkWidget *widget,
   GtkSelectionData *selection,
   guint      info,
   guint      time,
   gpointer   data)
#else 
void convertSelection(
   GtkWidget *widget,
   GtkSelectionData *selection,
   gpointer   data)
#endif
{
   int i;
   int width, height;
   char *str;
   guchar *text;
   gint len;
   gint format;
   GdkAtom type;

   /* no highlight */
   if(dataSelected != DATA_SELECTED){

      printf("convertSelection called, but no selection.\n");

   } else {

      width = hiX2 - hiX1 + 1;
      height = hiY2 - hiY1 + 1;
      len = (width + 1) * height;

      str = malloc(len * sizeof(*str));

      for(i=0; i<height; i++){
         strncpy(str+(i*(width+1)), &(lcdText[i+hiY1][hiX1]), width);
         *(str+(i*(width+1))+width) = '\n';
      }
      str[len-1] = '\0';

#ifdef GTK_VER_1_1
      switch(info){
#else
      switch(selection->target){
#endif
/*         case GDK_TARGET_STRING: */
         case TARGET_STRING:
	    gtk_selection_data_set(
	       selection,
               GDK_SELECTION_TYPE_STRING,
               8,
               (guchar*)str,
               len-1);
            break;
         case TARGET_TEXT:
         case TARGET_COMPOUND_TEXT:
	    gdk_string_to_compound_text(str,
               &type, &format, &text, &len);
	    gtk_selection_data_set(
               selection,
               type,
               format,
               text,
               len);
	    gdk_free_compound_text(text);
            break;
      }
   }
}


gint loseSelection(
   GtkWidget *widget,
   GdkEventSelection  *event)
{
   /* if there is a current (old) selection, then clear it */
   if(dataSelected == DATA_SELECTED){
      dataSelected = NONE_SELECTED;
      drawStackLCD();
   }
   return TRUE; 
}

void lcdStartHighlight(int x, int y, int time){
   int xx, yy;

   /* if there is a current (old) selection, then clear it */
   if(dataSelected == DATA_SELECTED){
      dataSelected = NONE_SELECTED;
      drawStackLCD();

      /* ... and tell GDK */
      if(gdk_selection_owner_get(GDK_SELECTION_PRIMARY) == lcdDA->window){
	 gtk_selection_owner_set(NULL,
            GDK_SELECTION_PRIMARY,
	    GDK_CURRENT_TIME);
      }
   }

   xx = (x - BORDER) / fontW;
   if(xx < 0) xx = 0;
   if(xx >= lcdWidth) xx = lcdWidth - 1;

   yy = (y - BORDER) / fontH;
   if(yy < 0) yy = 0;
   if(yy >= lcdHeight) yy = lcdHeight - 1;

   hiX0 = hiX1 = hiX2 = xx;
   hiY0 = hiY1 = hiY2 = yy;

   dataSelected = START_SELECTING;

}

void lcdStopHighlight(int x, int y, int time){
   int reCalc, curPos;
   int xx, yy;

   /* if the user started to select something, but didn't actually
      select anything, then reset stuff */
   if(dataSelected == START_SELECTING){
      dataSelected = NONE_SELECTED;

      /* user may have been positioning the edit cursor */
      if(isEditingEditor()){
	 xx = (x - BORDER) / fontW;
	 if(xx < 0) xx = 0;
	 if(xx >= lcdWidth) xx = lcdWidth - 1;
       
	 yy = (y  - BORDER) / fontH;
         if(yy == lcdHeight-1){


            curPos = cursorPosEditor();
	    if(curPos > lcdWidth){
               reCalc = 1;
	       xx += (curPos - lcdWidth);
	    } else {
               reCalc = 0;
            }
            setCursorPosEditor(xx);

            /* need to recalc since cursor may have moved out of the
               visable part of the editor */
            if(reCalc) calcStackLCD();
         }

      }

      /* clear any old highlights */
      drawStackLCD();
   }

   /* if data is now selected */
   if(dataSelected == DATA_SELECTED){

      if(0 == gtk_selection_owner_set(
         lcdDA,
         GDK_SELECTION_PRIMARY,
         GDK_CURRENT_TIME))
      {
         fprintf(stderr, "failed to gain selection\n");

	 dataSelected = NONE_SELECTED;
	 drawStackLCD();
      }
   }
}



void lcdContinueHighlight(int x, int y){
   int xx, yy;
   int lastx1, lastx2, lasty1, lasty2;

   xx = (x - BORDER) / fontW;
   if(xx < 0) xx = 0;
   if(xx >= lcdWidth) xx = lcdWidth - 1;

   yy = (y - BORDER) / fontH;
   if(yy < 0) yy = 0;
   if(yy >= lcdHeight) yy = lcdHeight - 1;

   lastx1 = hiX1;
   lastx2 = hiX2;
   lasty1 = hiY1;
   lasty2 = hiY2;

   if(xx < hiX0){
      hiX1 = xx;
      hiX2 = hiX0 + 0;
   } else {
      hiX1 = hiX0;
      hiX2 = xx + 0;
   }

   if(yy < hiY0){
      hiY1 = yy;
      hiY2 = hiY0 + 0;
   } else {
      hiY1 = hiY0;
      hiY2 = yy + 0;
   }

   /* if the highlight area changed */
   if(lastx1 != hiX1 || lastx2 != hiX2 || lasty1 != hiY1 || lasty2 != hiY2){

      dataSelected = DATA_SELECTED;

      /* repaint the display */
      drawStackLCD();
   }

}


/* ask for data */
void lcdGetSelection(int time){

  /* And request the primary selection */
  gtk_selection_convert(lcdDA,
     GDK_SELECTION_PRIMARY,  /* GdkAtom selection */
     GDK_TARGET_STRING,      /* GdkAtom target */
     GDK_CURRENT_TIME);      /* guint32 time */

}

void lcdPasteCB(
   GtkWidget *widget,
   GtkSelectionData *selection,
   gpointer data)
{
   int i;
   char *str;
   static GdkAtom clipboard_atom = GDK_NONE;

   if(selection->length <= 0 ||
      selection->type != GDK_SELECTION_TYPE_STRING){

      /* printf("length is zero or did not get a string.\n"); */

      if(selection->selection != GDK_SELECTION_PRIMARY) return;

      /* selection failed.  Try the clipboard now... */
      if(clipboard_atom == GDK_NONE){
	 clipboard_atom = gdk_atom_intern("CLIPBOARD", FALSE);
      }

      /* ask for another selection, but this time from the clipboard */
      gtk_selection_convert(lcdDA,
	clipboard_atom,         /* GdkAtom selection */
	GDK_TARGET_STRING,      /* GdkAtom target */
	GDK_CURRENT_TIME);      /* guint32 time */

      return;
   }

   str = (char*)selection->data;

   for(i=0; i<selection->length; i++){

      /* TODO maybe do something like isalpha() here? */
      processInput(0, 0, str[i], str[i]);
   }

}
