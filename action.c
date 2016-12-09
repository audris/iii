#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>

#include <X11/Xlib.h>
#include <X11/Intrinsic.h>

#include "iii.h"
#include "action.h"
#include "layout.h"

extern int LAYOUT_ROWS_OR_COLS;



selectIconRange (int from, int to, int reset)
{
	int i;
	selectIconBy (from, reset);
	if (from <= to)
		for (i = from + 1; i <= to; i++) 
			selectIconBy (i, 0);
	else
		for (i = from - 1; i >= to; i--) 
			selectIconBy (i, 0);
}

selectIconAt (int x, int y, int reset)
{
	int which = fromXYtoN (&one, x, y), i, xx, yy;
	selectIconBy (which, reset);
}

drawOutline (int which)
{
	int  i, xx, yy;
	XPoint lin [5];

	fromNtoXY (&one, which, &xx, &yy);
	lin [0] .x = xx + 1; 
	lin [0] .y = yy + 1; 
	lin [1] .x = xx + one .iconIm .width - 1; 
	lin [1] .y = yy + 1; 
	lin [2] .x = xx + one .iconIm .width - 1; 
	lin [2] .y = yy + one .iconIm .height - 1; 
	lin [3] .x = xx + 1; 
	lin [3] .y = yy + one .iconIm .height - 1; 
	lin [4] .x = xx + 1; 
	lin [4] .y = yy + 1; 
	XDrawLines (one .dis, one .pixmap, one .xorOutlinegc, lin,
					5, CoordModeOrigin);
	for (i = 0; i < 5; i++){
		lin [i] .x += 4;
		lin [i] .y += 2;
	}
	XDrawLines (one .dis, XtWindow(one .mainW), one .xorOutlinegc, lin,
					5, CoordModeOrigin);	
/* Put inverse image */
/*	fseek (one .iconIm .pgmFile, 
				one .iconIm .order [which] *
			 one .iconIm .width*one .iconIm .height
					 + one .iconIm .offset, SEEK_SET);
	fread (one .iconIm .data, one .iconIm .width * one .iconIm .height,
			 1, one .iconIm .pgmFile);
	XPutImage (one .dis, one .pixmap, one .highlightgc,
				  &(one .iconIm .template),
					0, 0, xx, yy,
					  one .iconIm .width, one .iconIm .height);
	XSetForeground (one .dis, one .highlightgc, 0);
	XFillRectangle (one .dis, XtWindow(one .mainW), one .highlightgc,
						 xx + 4, yy + 2,
						 one .iconIm .width, one .iconIm .height);
	XPutImage (one .dis, XtWindow(one .mainW), one .highlightgc,
				  &(one .iconIm .template),
					0, 0, xx+4, yy+2,
					  one .iconIm .width, one .iconIm .height);
*/
	printf (
"in selectIconBy at imOnSc=%d imInDat=%d - %d %d\n",
			  which, one .iconIm .order [which], xx, yy);	
}

selectIconBy (int which, int reset)
{
	int  i;
	if (reset){
		for (i = 0; i < one .NSelected; i++)
			drawOutline (one .iconIm .invOrder [one .SelectedIndex [i]]);
		one .NSelected = 1;
		one .AnimateTo
			= one .AnimateFrom = which;
		one .SelectedIndex [0] = one .iconIm .order [which];
		printf ("%.336s\n", one .data .records [one .iconIm .order [which]]);
	}else{
		int deselect = 0;
		for (i = 0; i < one .NSelected; i++)
			if (one .SelectedIndex [i] == one .iconIm .order [which]){
				deselect = 1;
				break;
			}
		if (!deselect){
			one .SelectedIndex [one .NSelected]  = one .iconIm .order [which];
			one .NSelected ++;
		}else{
			while (i < one .NSelected - 1){
				one .SelectedIndex [i] = one .SelectedIndex [i+1];
				i++;
			}
			one .NSelected --;
		}
	}
	drawOutline (which);
}


XtCallbackProc CtrlLeftClickAction (Widget w, XButtonEvent * event)
{
	int wid = event ->x / one .iconIm .width + 1;
	int hei = event ->y / one .iconIm .height + 1;
	fprintf (stderr, "In action CtrlLeftClickAction at %d %d\n",
				event ->x, event ->y, wid, hei);
	switch (one .typeOfLayout){
	case 0: /* across */
		LAYOUT_ROWS_OR_COLS = wid;
		break;			
	case 1: /* down */
		LAYOUT_ROWS_OR_COLS = hei;
		break;
	default:
		return;
		break;
	}
	LayoutMenuSelect (0, one .typeOfLayout, 0);
}

XtCallbackProc ShiftLeftClickAction (Widget w, XButtonEvent * event)
{
	fprintf (stderr, "In action ShiftLeftClickAction at %d %d\n",
				event ->x, event ->y);	
	selectIconAt (event ->x, event ->y, 0);	
}

XtCallbackProc LeftClickAction (Widget w, XButtonEvent * event)
{
	fprintf (stderr, "In action LeftClickAction at %d %d\n",
				event ->x, event ->y);
	selectIconAt (event ->x, event ->y, 1);	
}


XtCallbackProc MiddleClickAction (Widget w, XButtonEvent * event)
{
	char buff [299];
	int hNum = one .width / one .iconIm .width,
			 vNum = one .height / one .iconIm .height;
	int x, y, ii;
	int which = fromXYtoN (&one, event ->x, event ->y);
	if (which >= one .NObjects ||
		 event ->x >= one .iconIm .width * hNum ||
		 event ->y >= one .iconIm .height * vNum)
		return 0;
	waiting (&one);
	ii = one .iconIm .order [which];
	{
		int offset = one .LVRindex [ii];
		sprintf (buff, "%s/lvrShow %d", getenv ("PLANETS_BASE"),
				offset);
		printf ("%s\n", buff); 
		printf ("%.8s ii = %d, offset = %d\n",
				  one .data .records [one .iconIm .order [which]]
				  .text .fields .body, ii, offset);
				  
	}
	system (buff);
/*	selectIconAt (event ->x, event ->y, 0);*/
	doneWaiting (&one);
}


XtCallbackProc Middle2ClickAction (Widget w, XButtonEvent * event)
{
	char buff [299];
	int hNum = one .width / one .iconIm .width,
			 vNum = one .height / one .iconIm .height;
	int x, y, ii;
	int which = fromXYtoN (&one, event ->x, event ->y);
	printf ("in action Middle2ClickAction\n");
	if (which >= one .NObjects ||
		 event ->x >= one .iconIm .width * hNum ||
		 event ->y >= one .iconIm .height * vNum)
		return 0;
/*	MiddleClickAction (w, event);*/	
	waiting (&one);
	ii = one .iconIm .order [which];
	sprintf (buff, "%s/cdShow %d", getenv ("PLANETS_BASE"),
				one .CDindex [ii]);
	fprintf (stderr, "%s\n", buff);
	system (buff);
	doneWaiting (&one);	
}




XtCallbackProc ShiftMiddleClickAction (Widget w, XButtonEvent * event)
{
	char buff [299];
	int hNum = one .width / one .iconIm .width,
			 vNum = one .height / one .iconIm .height;
	int which = fromXYtoN (&one, event ->x, event ->y);
	printf ("in action ShiftMiddleClickAction\n");	
	if (which >= one .NObjects ||
		 event ->x >= one .iconIm .width * hNum ||
		 event ->y >= one .iconIm .height * vNum)
		return 0;
	waiting (&one);
	one .AnimateFrom = which;
	if (one .AnimateTo < 0)
		one .AnimateTo = one .AnimateFrom;
	else
		selectIconRange (one .AnimateFrom, one .AnimateTo, 1);
	doneWaiting (&one);
}

XtCallbackProc ShiftRightClickAction (Widget w, XButtonEvent * event)
{
	char buff [299];
	int hNum = one .width / one .iconIm .width,
			 vNum = one .height / one .iconIm .height;
	int which = fromXYtoN (&one, event ->x, event ->y);
	printf ("in action ShiftRightClickAction\n");	
	if (which >= one .NObjects ||
		 event ->x >= one .iconIm .width * hNum ||
		 event ->y >= one .iconIm .height * vNum)
		return 0;
	waiting (&one);
	one .AnimateTo = which;
	if (one .AnimateFrom < 0)
		one .AnimateFrom = one .AnimateTo;
	else
		selectIconRange (one .AnimateFrom, one .AnimateTo, 1);
	doneWaiting (&one);
}

XtCallbackProc CtrlRightClickAction (Widget w, XButtonEvent * event)
{
	char buff [1000];
	int i;
	printf ("in action CtrlRightClickAction\n");
	if (one .NSelected <= 0)
		return 0;		
	waiting (&one);	
	{
		char nam [L_tmpnam];
		FILE * ind;
		tmpnam (nam);
		ind = fopen (nam, "w");
		for (i = 0; i < one .NSelected; i++)
			fprintf (ind, "%d\n", one .SelectedIndex [i]);
		fclose (ind);
		sprintf (buff, "%s/iii.bin %s %s &", getenv("PLANETS_BASE"),
					nam, one .TheName);
		system (buff);
	}
	doneWaiting (&one);
}

XtCallbackProc RightClickAction (Widget w, XButtonEvent * event)
{
	char buff [1000];
	int i;
	int hNum = one .width / one .iconIm .width,
			 vNum = one .height / one .iconIm .height;
	printf ("in action RightClickAction\n");
	if (one .AnimateFrom < 0 || one .AnimateTo < 0)
		return 0;
	waiting (&one);

	if (one .NSelected)
		for (i = 0; i <  one .NSelected; i++){
			int offset = one .LVRindex [one .SelectedIndex [i]];
			sprintf (buff, "%s/lvrShow %d", getenv("PLANETS_BASE"),
						offset);
			system (buff);
		}
	doneWaiting (&one);
}

XtCallbackProc Right2ClickAction (Widget w, XButtonEvent * event)
{
	char buff [299];
	int i;
	int hNum = one .width / one .iconIm .width,
			 vNum = one .height / one .iconIm .height;
	printf ("in action Right2ClickAction\n");	
	waiting (&one);
	if (one .AnimateFrom <= one .AnimateTo)
		for (i = one .AnimateFrom; i <=  one .AnimateTo; i++){
			int ii = one .CDindex [one .iconIm .order [i]];
			sprintf (buff, "%s/cdGet %d", getenv("PLANETS_BASE"),
						ii);
			system (buff);
		}
	doneWaiting (&one);
}

XtCallbackProc ExposeAction (Widget w, XButtonEvent * event)
{
	printf ("in action exp %d %d\n", event ->x, event ->y);
}

XtCallbackProc Quit (Widget w, XtPointer ooo)
{
	closeGraph ((iii *)ooo);
	exit (0);
}

XtCallbackProc Help (Widget w, XtPointer onePt)
{
	char buf [1000];
	iii * ooo =  ((iii *)onePt);
	
	sprintf (buf, "%s/Description", getenv ("PLANETS_BASE"));
	help (ooo, buf);
}





