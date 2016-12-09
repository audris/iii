#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <sys/types.h>
#include <X11/Xlib.h>
#include <X11/X.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>
#include <X11/cursorfont.h>
#include <X11/Intrinsic.h> 
#include <X11/Shell.h> 
#include <X11/StringDefs.h>
#include <X11/Xaw/Paned.h>
#include <X11/Xaw/Label.h>
#include <X11/Xaw/Box.h>
#include <X11/Xaw/Scrollbar.h>
#include <X11/Xaw/Command.h>
#include <X11/Xaw/MenuButton.h>
#include <X11/Xaw/SimpleMenu.h>
#include <X11/Xaw/SmeBSB.h>
#include <X11/Xaw/Viewport.h>


#include "iii.h"	
#include "menu.h"	
#include "action.h"	
#include "order.h"	
#include "layout.h"	

iii one;
static XtTranslations tr;
static XtActionsRec actionsTable [] = {
		{(String)"LeftClickAction",   (XtActionProc)LeftClickAction},
		{(String)"ShiftLeftClickAction",   (XtActionProc)ShiftLeftClickAction},
		{(String)"CtrlLeftClickAction",   (XtActionProc)CtrlLeftClickAction},
		{(String)"MiddleClickAction",   (XtActionProc)MiddleClickAction},
		{(String)"Middle2ClickAction",   (XtActionProc)Middle2ClickAction},
		{(String)"ShiftMiddleClickAction", (XtActionProc)ShiftMiddleClickAction},
		{(String)"ShiftRightClickAction", (XtActionProc)ShiftRightClickAction},
		{(String)"CtrlRightClickAction", (XtActionProc)CtrlRightClickAction},
		{(String)"RightClickAction", (XtActionProc)RightClickAction},
		{(String)"Right2ClickAction", (XtActionProc)Right2ClickAction},
		{(String)"ExposeAction",   (XtActionProc)ExposeAction},
};

void SetupMainWindow ()
{
	if (one .mainW != 0){
		XtDestroyWidget (one .mainW);
		XFreePixmap (one .dis, one .pixmap);
	}
	one .mainW = XtVaCreateManagedWidget("III",labelWidgetClass,
		one .form,
		XtNheight, one .height, XtNwidth, one .width,
		XtNx, 0, XtNy, 0,
		NULL);
	XtVaSetValues (one .mainW,  XtNcursor, one .Cross, 0);
	XtVaSetValues (one .form, XtNtranslations, tr,	NULL);
	one .pixmap = XCreatePixmap (one .dis, XtWindow (one .mainW),
									  one .width, one .height,
									  one .depth);
	XtVaSetValues (one .mainW, XtNbitmap, one .pixmap,
					NULL);
}


#define myWhite 0
#define myBlack 1
#define MIN(a, b) ((a) > (b) ? (b) : (a))
#define MAX(a, b) ((a) < (b) ? (b) : (a))

static myXerror = 0;
int errH (Display * dis, XErrorEvent *err);
static char bits[] = {
   0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xff,
   0xff, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x3f, 0xff, 0xff, 0xff,
   0xff, 0xff, 0xff, 0x3f, 0xff, 0xff, 0xfe, 0xff, 0xff, 0xff, 0x3f, 0xff,
   0x3f, 0xfe, 0x7f, 0xc0, 0xff, 0x3f, 0x1f, 0x00, 0xff, 0x7f, 0xc0, 0xff,
   0x3f, 0x0f, 0x80, 0xff, 0x7f, 0xc0, 0xff, 0x3f, 0x07, 0xc0, 0xff, 0xff,
   0xf1, 0xff, 0x3f, 0x1f, 0xfc, 0xff, 0xff, 0xf1, 0xff, 0x3f, 0x3f, 0xfc,
   0xff, 0xff, 0xf1, 0xff, 0x3f, 0x7f, 0xfc, 0xff, 0xff, 0xf1, 0xff, 0x3f,
   0x7f, 0xfc, 0xcf, 0xff, 0xf1, 0xff, 0x3f, 0x7f, 0xfc, 0xc3, 0xff, 0xf1,
   0xff, 0x3f, 0x7f, 0xfc, 0xe0, 0xff, 0xf1, 0xff, 0x3f, 0x7f, 0x7c, 0xf0,
   0xff, 0xf1, 0xff, 0x3f, 0x7f, 0x3c, 0xf0, 0xff, 0xf1, 0xff, 0x3f, 0x7f,
   0x3c, 0xf0, 0xff, 0xf1, 0xff, 0x3f, 0x7f, 0xfc, 0xf0, 0xff, 0xf1, 0xff,
   0x3f, 0x7f, 0x7c, 0xf8, 0xff, 0xf1, 0xff, 0x3f, 0x7f, 0x7c, 0xf0, 0xff,
   0xf1, 0xff, 0x3f, 0x3f, 0x7c, 0xf0, 0xff, 0xf1, 0xff, 0x3f, 0x7f, 0x7c,
   0xf0, 0xff, 0xf1, 0xff, 0x3f, 0x7f, 0x7c, 0xf0, 0xff, 0xf1, 0xff, 0x3f,
   0x3f, 0xfc, 0xf0, 0xff, 0xf1, 0xff, 0x3f, 0x3f, 0xfc, 0xf0, 0xff, 0xf1,
   0xff, 0x3f, 0x1d, 0xfe, 0xf0, 0xff, 0xf1, 0xff, 0x3f, 0x09, 0xff, 0xf0,
   0xff, 0xf1, 0xff, 0x3f, 0x01, 0xff, 0xf0, 0xff, 0xf1, 0xff, 0x3f, 0x03,
   0xff, 0xf8, 0xff, 0xf1, 0xff, 0x3f, 0x07, 0xff, 0xf8, 0xff, 0xf1, 0xff,
   0x3f, 0x07, 0xfe, 0xf8, 0xff, 0xf1, 0xff, 0x3f, 0x1f, 0x7c, 0xf8, 0xff,
   0xf1, 0xff, 0x3f, 0x1f, 0x78, 0xf8, 0xff, 0xf1, 0xff, 0x3f, 0x7f, 0x38,
   0xf8, 0xff, 0xf1, 0xff, 0x3f, 0xff, 0x3f, 0xf8, 0xfe, 0xf1, 0xff, 0x3f,
   0xff, 0x3f, 0x50, 0xff, 0xf1, 0xff, 0x3f, 0xff, 0x1f, 0x00, 0xff, 0xf1,
   0xff, 0x3f, 0xff, 0x0f, 0x80, 0xff, 0xf1, 0xff, 0x3f, 0xff, 0x07, 0xe0,
   0xff, 0xf1, 0xff, 0x3f, 0xff, 0x83, 0xff, 0xff, 0xf1, 0xff, 0x3f, 0xff,
   0xfb, 0xff, 0xff, 0xf1, 0xff, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xf1, 0xff,
   0x3f, 0xff, 0xff, 0xff, 0xff, 0xf1, 0xff, 0x3f, 0xff, 0xff, 0xff, 0xff,
   0xf1, 0xff, 0x3f, 0xff, 0xff, 0xff, 0x7f, 0xc0, 0xff, 0x3f, 0xff, 0xff,
   0xff, 0x7f, 0xc0, 0xff, 0x3f, 0xff, 0xff, 0xff, 0x7f, 0xc0, 0xff, 0x3f,
   0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xff,
   0xff, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x3f, 0xff, 0xff, 0xff,
   0xff, 0xff, 0xff, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x3f, 0xff,
   0xff, 0xff, 0xff, 0xff, 0xff, 0x3f};

Boolean StrToColor (Display * dis, XrmValuePtr args, Cardinal * nargs,
						  XrmValuePtr from, XrmValuePtr to)
{
	static XColor col;
	static Pixel p;
/*	fprintf (stderr, "in converter %s\n", from ->addr);*/
	if (!strcasecmp (from ->addr, "white") ||
		 !strcasecmp (from ->addr, "XtDefaultForeground")){
		p = one .foreground;
		col .pixel = 0xff;
		col .red = 0xff * 0xff; 
		col .green = 0xff * 0xff ;
		col .blue = 0xff * 0xff ;
		col .flags = DoRed | DoGreen | DoBlue;
	}else{
		p = 0;
		col .pixel = one .background;;
		col .red = 0;
		col .green = 0;
		col .blue = 0;
		col .flags = DoRed | DoGreen | DoBlue;
	}
	to ->size = sizeof (Pixel);
 	to ->addr = (XtPointer)(&p);
	return TRUE;
}

void initGraph ()
{
	static first = 1;
	int i,  nOfEntries;
	XGCValues val;
	XSetWindowAttributes attr;
	XVisualInfo *list, templ;
	char * fontname = /*"my12"*/"12x24";
	XSizeHints size_hints;	
	
	if (!first){
/*		XFillRectangle (one .dis, one .pixmap, one .myblackgc, 0, 0, w, h);*/
		return;
	}else{
		one .im = NULL;
		one .top = one .left = 0;
		one .fontInfo = NULL;
		one .DisplayName = NULL;
		one .dis = NULL;
	}
	first = 0;
	
	{
		int i = 0;
		XtToolkitInitialize ();
		one .appCon = XtCreateApplicationContext ();
		XtAppSetTypeConverter (one .appCon,
									  XtRString, XtRPixel,
								     (XtTypeConverter)StrToColor,
									  (XtConvertArgList)NULL,
									  0,
									  XtCacheAll, NULL);  
		if ((one .dis = XtOpenDisplay (one .appCon, one .DisplayName, "III",
							"ICE CUBE", NULL, 0, &i, NULL)) == NULL){
			printf ("Can not open Display\n");
			exit (-1);
		}
	}
	one .screen = XDefaultScreen (one .dis);
	one .depth = XDefaultDepth (one .dis, one .screen);
	one .defCmap = XDefaultColormap (one .dis, one .screen); 
	one .rwin = RootWindow(one .dis, one .screen);
	one .vis = XDefaultVisual (one .dis, one .screen);
   one .nOfEntries = XDisplayCells (one .dis, one .screen);

  	XSetErrorHandler (errH);
	XSynchronize (one .dis, True);
	one .vis = XDefaultVisual (one .dis, one .screen);
	if ((one .fontInfo = XLoadQueryFont(one .dis, fontname))==NULL){
		fprintf(stderr,"Cannot open %s font\n", fontname);
	}
	{
		int nitems;
		templ .screen = one .screen;
		templ .depth = 24;
		templ .class = DirectColor;
	
		list = XGetVisualInfo (one .dis, VisualScreenMask | VisualClassMask
								  | VisualDepthMask,
								  &templ, &nitems);
		if (nitems){
				one .trueVis = list [0] .visual;
				one .vis = list [0] .visual;
				one .depth = templ .depth;;
			}else{
				fprintf (stderr,
				"No visuals at depth 24, using default visual at depth %d\n",
							one .depth);
			}
	}
/*	{
		int nitems;
		templ .screen = one .screen;
		templ .depth = 8;
		templ .class = PseudoColor;
		templ .colormap_size = 0x100;
		
		list = XGetVisualInfo (one .dis, VisualScreenMask | VisualClassMask
								  | VisualDepthMask | VisualColormapSizeMask,
								  &templ, &nitems);
		if (nitems){
				one .trueVis = list [0] .visual;
				one .vis = list [0] .visual;
				one .depth = templ .depth;
				one .nOfEntries = templ .colormap_size;
			}else{
				fprintf (stderr,
"No visuals at depth 8 with 256 colors, using default visual at depth %d\n",
							one .depth);
			}
	}*/
	switch (one .depth){
	case 1:
		one .cmap = XCreateColormap (one .dis, one .rwin, one .vis, AllocNone);
		one .foreground = XWhitePixel (one .dis, one .screen);
		one .background = XBlackPixel (one .dis, one .screen);			
		break;
	case 24:
		one .foreground = 255 + (255 << 8) + (255 << 16);
		one .background = 150 ;			
		one .cmap = XCreateColormap (one .dis, one .rwin, one .vis, AllocAll);
		break;
	case 8:
		one .foreground = one .nOfEntries - 1;
		one .background = 0;			
		one .color = (XColor *) malloc (one .nOfEntries * sizeof (XColor));
		makeColorMap (one .color, "gray");	
		one .cmap = XCreateColormap (one .dis, one .rwin, one .vis, AllocAll);
		if (0==XStoreColors (one .dis, one .cmap, one .color, one .nOfEntries))
			;
		break;
	}

/********* WIDGETS *************************/	
	one .toplevel = XtVaAppCreateShell ("III", "ICE CUBE",
		applicationShellWidgetClass,
		one .dis,
		XtNdepth, one .depth, 				  
		XtNcolormap, one .cmap,
		XtNvisual, one .vis,
		XtNiconPixmap,  XCreateBitmapFromData (one .dis, one .rwin,
															bits, 54, 54),
/*		XtNmaxHeight, XDisplayHeight (one .dis, one .screen) - 330,
		XtNmaxWidth, XDisplayWidth (one .dis, one .screen) - 330,
		XtNminHeight, XDisplayHeight (one .dis, one .screen) - 330,
		XtNminWidth, XDisplayWidth (one .dis, one .screen) - 330,
		XtNx, 0, XtNy, 0,
      XtNgeometry, "+0+0",
*/		NULL);
	
	one .vpane = XtVaCreateManagedWidget("vpane", panedWidgetClass,
		one .toplevel,
		XtNinternalBorderColor, one .foreground, 		
		NULL);
	one .bb = XtVaCreateManagedWidget("buttonbox", boxWidgetClass,
		one .vpane,
		NULL);

/******** MENU ********************/	
	one .out = XtVaCreateManagedWidget("Exit", commandWidgetClass,
		one .bb,
		NULL);
	XtAddCallback (one .out, XtNcallback, (XtCallbackProc)Quit,
						(XtPointer *)&one);
	one .help = XtVaCreateManagedWidget("Help", commandWidgetClass,
		one .bb,
		NULL);
	XtAddCallback (one .help, XtNcallback, (XtCallbackProc)Help,
						(XtPointer *)&one);
	one .readPageButton =
		XtVaCreateManagedWidget("Read Page", menuButtonWidgetClass,
		one .bb,
		XtNmenuName, "ReadPageMenu",
		NULL);
	one .readPage = XtVaCreatePopupShell("ReadPageMenu", simpleMenuWidgetClass,
		one .readPageButton,
		XtNdepth, one .depth, 				  
		XtNcolormap, one .cmap,
		XtNvisual, one .vis,
		NULL);
	for (i = 0; i < XtNumber (ReadPageMenuItems); i++){
		Widget entry = XtVaCreateManagedWidget(ReadPageMenuItems [i],
			smeBSBObjectClass,
			one .readPage,
			NULL);
		XtAddCallback(entry, XtNcallback, ReadPageMenuSelect, (XtPointer) i);
	}
	one .layoutButton = XtVaCreateManagedWidget("Layout", menuButtonWidgetClass,
		one .bb,
		XtNmenuName, "LayoutMenu",
		NULL);
	one .layout = XtVaCreatePopupShell("LayoutMenu", simpleMenuWidgetClass,
		one .layoutButton,
		XtNdepth, one .depth, 				  
		XtNcolormap, one .cmap,
		XtNvisual, one .vis,
		NULL);
	for (i = 0; i < XtNumber (LayoutMenuItems); i++){
		Widget entry = XtVaCreateManagedWidget(LayoutMenuItems [i],
			smeBSBObjectClass,
			one .layout,
			NULL);
		XtAddCallback(entry, XtNcallback, LayoutMenuSelect, (XtPointer) i);
	}
	one .orderButton = XtVaCreateManagedWidget("Order", menuButtonWidgetClass,
		one .bb,
		XtNmenuName, "OrderMenu",
		NULL);
	one .order = XtVaCreatePopupShell("OrderMenu", simpleMenuWidgetClass,
		one .orderButton,
		XtNdepth, one .depth, 				  
		XtNcolormap, one .cmap,
		XtNvisual, one .vis,
		NULL);
	for (i = 0; i < XtNumber (OrderMenuItems); i++){
		Widget entry = XtVaCreateManagedWidget(OrderMenuItems [i],
			smeBSBObjectClass,
			one .order,
			NULL);
		XtAddCallback(entry, XtNcallback, OrderMenuSelect, (XtPointer) i);
	}
/*	one .clusterButton =
		XtVaCreateManagedWidget("Cluster", menuButtonWidgetClass,
		one .bb,
		XtNmenuName, "ClusterMenu",
		NULL);	
	one .cluster = XtVaCreatePopupShell("ClusterMenu", simpleMenuWidgetClass,
		one .clusterButton,
		XtNdepth, one .depth, 				  
		XtNcolormap, one .cmap,
		XtNvisual, one .vis,
		NULL);
	for (i = 0; i < XtNumber (ClusterMenuItems); i++){
		Widget entry = XtVaCreateManagedWidget(ClusterMenuItems [i],
			smeBSBObjectClass,
			one .cluster,
			NULL);
		XtAddCallback(entry, XtNcallback, ClusterMenuSelect, (XtPointer) i);
	}
*/

/************* Image area ************************************************/	
	one .view = XtVaCreateManagedWidget("view", viewportWidgetClass, one .vpane,
		XtNallowHoriz, True, XtNallowVert, True,
		NULL);
	one .form = XtVaCreateManagedWidget("form", boxWidgetClass, one .view,
		XtNhSpace, 0,XtNvSpace, 0,				
		NULL);

/* bogus starting widow */	
	one .typeOfLayout = -1;
	one .width = XDisplayWidth (one .dis, one .screen) - 350;
	one .height = XDisplayHeight (one .dis, one .screen) - 350;
	one .mainW = XtVaCreateManagedWidget("III", labelWidgetClass, one .form,
		XtNheight, one .height, XtNwidth, one .width,
		XtNx, 0, XtNy, 0,
		NULL);

	XtAppAddActions(one .appCon, actionsTable, XtNumber (actionsTable));
	tr = XtParseTranslationTable (
		  "Shift <Btn2Down>     : ShiftMiddleClickAction()\n\
         <Btn2Down>(2)        : Middle2ClickAction()    \n\
         <Btn2Down>           : MiddleClickAction()     \n\
         Shift <Btn3Down>     : ShiftRightClickAction() \n\
         Ctrl <Btn3Down>      : CtrlRightClickAction()  \n\
         <Btn3Down>(2)        : Right2ClickAction()     \n\
         <Btn3Down>           : RightClickAction()      \n\
         Shift <Btn1Down>     : ShiftLeftClickAction()  \n\
         Ctrl <Btn1Down>      : CtrlLeftClickAction()   \n\
         <Btn1Down>           : LeftClickAction()       \n\
			<Expose>             :   ExposeAction()");
	one .Wait = XCreateFontCursor (one .dis, XC_watch);
	one .Cross = XCreateFontCursor (one .dis, XC_cross);

	XtRealizeWidget(one .toplevel);

/* adding scrollbar controll */	
/*	one .horizontal = XtNameToWidget (one .view, "horizontal");
	one .vertical = XtNameToWidget (one .view, "vertical");
	XtVaSetValues (one .horizontal,
		XtNforeground, one .foreground,
		XtNborderColor, one .foreground, 		
		XtNbackground, one .background, 				  
		NULL);
	XtVaSetValues (one .vertical,
		XtNforeground, one .foreground,
		XtNborderColor, one .foreground, 		
		XtNbackground, one .background, 				  
		NULL);
*/	
/* GC, STIPPLES, etc. */
	one .stiple = XCreatePixmap (one .dis, XtWindow (one .mainW), 9, 9, 1);	
	one .currentgc = XCreateGC (one .dis, XtWindow (one .mainW),0,&val);
	one .bitgc    =  XCreateGC (one .dis, one .stiple, 0,&val);
	one .stip1gc = XCreateGC (one .dis,XtWindow (one .mainW),0,&val);
	if (one .depth == 24)
		val .plane_mask = (0xff << 16) + (0xff << 8);
	else
		val .plane_mask = 0xf << 2;			
	val .function = GXcopyInverted;
	val .graphics_exposures = True;
	one .highlightgc = XCreateGC (one .dis, XtWindow (one .mainW),
		/*GCGraphicsExposures | */GCFunction | GCPlaneMask, &val);

	one .xorgc = XCreateGC (one .dis, XtWindow (one .mainW), 0, &val);	
	XSetLineAttributes (one .dis, one .xorgc, 1, LineSolid,
							  CapNotLast, JoinBevel);
	XSetFunction (one .dis, one .xorgc, GXxor);
	XSetForeground (one .dis, one .xorgc, 255);
	one .xorOutlinegc = XCreateGC (one .dis, XtWindow (one .mainW), 0, &val);	
	XSetLineAttributes (one .dis, one .xorOutlinegc, 4, LineSolid,
							  CapNotLast, JoinBevel);
	XSetFunction (one .dis, one .xorOutlinegc, GXxor);
	XSetForeground (one .dis, one .xorOutlinegc, 255);


	
	XSetLineAttributes (one .dis, one .currentgc, 1, LineSolid,
							  CapButt, JoinMiter);
	XSetLineAttributes (one .dis, one .stip1gc, 1, LineSolid,
							  CapButt, JoinMiter);
	XSetForeground (one .dis, one .bitgc, 1);
	one .textgc = XCreateGC (one .dis, XtWindow (one .mainW), 0, &val);
	XSetForeground (one .dis, one .textgc, WhitePixel (one .dis, one .screen));
  	if (one .fontInfo != NULL)
		XSetFont(one .dis, one .textgc, one .fontInfo->fid);

	{
		int i, j;
		XSetForeground (one .dis, one .bitgc, 1);
		XFillRectangle (one .dis, one .stiple, one .bitgc, 0, 0, 9, 9);
		XSetForeground (one .dis, one .bitgc, 0);
		XDrawPoint (one .dis, one .stiple, one .bitgc, 0, 0);
		XDrawPoint (one .dis, one .stiple, one .bitgc, 0, 1);
		
/*		for (i = 0; i < 4; i++)
			for (j = 0; j <1; j++)
				XDrawPoint (one .dis, one .stiple, bitgc, i, j);
*/		XSetStipple (one .dis, one .stip1gc, one .stiple);
		XSetFillStyle (one .dis, one .stip1gc, FillStippled);
	}
	one .pixmap = XCreatePixmap (one .dis, XtWindow (one .mainW),
									  one .width, one .height,
									  one .depth);	
/*	clearScreen (one .background);*/
}

void closeGraph (iii * ooo)
{
	int i;

	if (ooo ->NObjects){
		DestroyXImageArray (&(ooo ->iconIm));
		free (ooo ->SelectedIndex);
		free (ooo ->LVRindex);
		free (ooo ->CDindex);
		free (ooo ->data .records);
	}
	if (ooo ->pixmap)
		XFreePixmap (ooo ->dis, ooo ->pixmap);
	if (ooo ->cmap)
		XFreeColormap (ooo ->dis, ooo ->cmap);
	if (ooo ->color)
		free (ooo ->color);
	XtDestroyWidget (ooo ->toplevel);
	XtCloseDisplay (ooo ->dis);
}


void flushGraph (void)
{
	int wid, hei;
	XCopyArea (one .dis, one .pixmap, XtWindow (one .mainW), one .currentgc,
	0, 0, one .width, one .height, one .left + 4, one .top + 2);

}
				  
void drawLine (int col, int length, short * line)
{
	int i;
	XSetForeground (one .dis, one .currentgc, col);
	XDrawLines (one .dis, one .pixmap, one .currentgc, (XPoint *)line,
					length, CoordModeOrigin);
/*	for (i=0; i< length; i++)
		fprintf(stderr, "%d %d,",line[2*i],line[2*i+1]);
	fprintf(stderr, "\n");
*/
}

void fillLine (int col, int length, short * line)
{
	if (col > 255){
		XSetForeground (one .dis, one .stip1gc, col - 255);
		XFillPolygon (one .dis, one .pixmap, one .stip1gc, (XPoint *)line, length, Nonconvex,
					  CoordModeOrigin);
	}
	else{
		XSetForeground (one .dis, one .currentgc, col);
		XFillPolygon(one .dis, one .pixmap, one .currentgc, (XPoint *)line, length, Nonconvex,
						 CoordModeOrigin);
	}
}

void drawPoints (int col, int length, short * line)
{
	XSetForeground (one .dis, one .currentgc, col);
	XDrawPoints (one .dis, one .pixmap, one .currentgc, (XPoint *)line, length, CoordModeOrigin);
}
						 





	
int errH (Display * dis, XErrorEvent *err)
{
	char buf [200];
	fprintf(stderr, 
"code=%d, text:%s\nMajor code:%d,\nMinor Code:%d,\nType:%d,\nserial:%ld,\nresourceId:%lu\n",
			 (unsigned)err ->error_code,
			 XGetErrorText (dis, err ->error_code, buf, 200),
			 (unsigned)err ->request_code,
			 (unsigned)err ->minor_code,
			 err ->type,
			 err ->serial,
			 err ->resourceid);
	myXerror = 1;
	exit (1);
}

XImage * CreateXImage (int width, int height)
{
	XImage * im;
	if (one .depth > 1)
		 im =
			XGetImage (one .dis, one .pixmap, 0, 0, width, height, -1, ZPixmap);
	else
		im =
			XGetImage (one .dis, one .pixmap, 0, 0, width, height, -1, XYPixmap);
	return im;
}

gray (XColor* color)
{
	int i, r, g, b;

/*	color [0] .pixel = 1;
	color [0] .red = 0xffff;
	color [0] .green = 0xffff;
	color [0] .blue = 0xffff;
	color [0] .flags = DoRed | DoGreen | DoBlue;

	color [1] .pixel = 0;
	color [1] .red = 0;
	color [1] .green = 0;
	color [1] .blue = 0;
	color [1] .flags = DoRed | DoGreen | DoBlue;
*/
	for (i = 0; i < 0x100; i++){
		color [i] .pixel = i;
		color [i] .red = (unsigned) i * 0xff;
		color [i] .green = (unsigned) i * 0xff;
		color [i] .blue = (unsigned) i * 0xff;
		color [i] .flags = DoRed | DoGreen | DoBlue;
	}
}

makeColorMap (XColor * color, char * name)
{
	if (strncasecmp (name, "gray", 4)==0){
		gray (color);
		return 1;
	}
	
	{
		FILE * clr = fopen (name, "r");
		int i;
		float r, g, b;
	
		if (clr == NULL){
			perror ("Can  not open color file!");
			exit (1);
		}

		color [0] .pixel = 0;
		color [0] .red = 0xffff;
		color [0] .green = 0xffff;
		color [0] .blue = 0xffff;
		color [0] .flags = DoRed | DoGreen | DoBlue;

		

		while (!feof (clr)){
			fscanf (clr, "%d %f %f %f", &i, &r, &g, &b);
			color [i] .pixel = i;
			color [i] .red = r * 0xffff;
			color [i] .green = g * 0xffff;
			color [i] .blue = b * 0xffff;
			color [i] .flags = DoRed | DoGreen | DoBlue;
		}
/* stuff for contours */		
/*		for (i = 2; i < NUMBER_OF_COLORS; i+=NUMBER_OF_COLORS/8){
			color [i] .pixel = i;
			color [i] .red = 0;
			color [i] .green = 0;
			color [i] .blue = 0;
			color [i] .flags = DoRed | DoGreen | DoBlue;
		}
*/
/* stuff for bacground */		
		color [1] .pixel = 1;
		color [1] .red = 0;
		color [1] .green = 51*0xffff*2.55;
		color [1] .blue = 66*0xffff*2.55;
		color [1] .flags = DoRed | DoGreen | DoBlue;
	}
}


/* Text manipulations */
void text (int x, int y, char * str)
{
	XDrawString(one .dis, one .pixmap, one .textgc, x, y, str, strlen(str));
}

void setTextAtt (int color, char * font_name)
{
  	if (one .fontInfo != NULL)
		XFreeFont (one .dis, one .fontInfo);
	if ((one .fontInfo = XLoadQueryFont(one .dis, font_name))==NULL){
		fprintf (stderr,"Cannot open %s font\n",font_name);
	}else{	
		XSetForeground (one .dis, one .textgc, color);
		XSetFont(one .dis, one .textgc, one .fontInfo ->fid);
	}
}

void clearScreen (int col)
{
	XSetForeground (one .dis, one .currentgc, col);
	XFillRectangle (one .dis, one .pixmap, one .currentgc, 0, 0,
						 one .width, one .height);
	XFillRectangle (one .dis, XtWindow (one .mainW), one .currentgc, 0, 0,
						 one .width, one .height);
}






