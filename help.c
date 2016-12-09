#include <stdio.h>

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xresource.h>
#include <X11/Intrinsic.h>
#include <X11/IntrinsicP.h>
#include <X11/Core.h>         /* tourlabels */
#include <X11/CoreP.h>         /* tourlabels */
#include <X11/StringDefs.h>
#include <X11/Shell.h>
#include <X11/cursorfont.h>
#include <X11/bitmaps/target>

#include <X11/Xaw/AsciiText.h>
#include <X11/Xaw/AsciiTextP.h>
#include <X11/Xaw/Box.h>
#include <X11/Xaw/BoxP.h>
#include <X11/Xaw/Cardinals.h>
#include <X11/Xaw/Command.h>
#include <X11/Xaw/CommandP.h>
#include <X11/Xaw/Dialog.h>
#include <X11/Xaw/DialogP.h>
#include <X11/Xaw/Form.h>
#include <X11/Xaw/FormP.h>
#include <X11/Xaw/Label.h>
#include <X11/Xaw/LabelP.h>
#include <X11/Xaw/MenuButton.h>
#include <X11/Xaw/Paned.h>
#include <X11/Xaw/PanedP.h>
#include <X11/Xaw/Scrollbar.h>
#include <X11/Xaw/ScrollbarP.h>
#include <X11/Xaw/SimpleMenu.h>
#include <X11/Xaw/Sme.h>
#include <X11/Xaw/SmeBSB.h>
#include <X11/Xaw/Toggle.h>
#include <X11/Xaw/ToggleP.h>

typedef struct {
  Widget hpopup, frame, text, done;
  int helpid;
} HelpData;

#include "iii.h"


XtCallbackProc HelpDoneCback (Widget w, XtPointer *dat, caddr_t cback_data)
{
  XtDestroyWidget (XtParent (XtParent (w)));
}

void help (iii * ooo, char * helpfile)
{
  int i, ok;
  HelpData *data;
  if (fopen (helpfile, "r") == NULL){
    (void) fprintf (stderr,
      "Unable to open %s.\n", helpfile);
    return;
  }else{
    ok = 0;
    data = (HelpData *) XtMalloc (sizeof(HelpData));
  /*
   * Create the popup itself.
  */
    data ->hpopup = XtVaCreatePopupShell ("Help",
      topLevelShellWidgetClass,														
      ooo ->toplevel,
      XtNdepth, ooo ->depth, 				  
		XtNcolormap, ooo ->cmap,
		XtNvisual, ooo ->vis,
		XtNtitle, (String) "III Help Window",
      XtNiconName, (String) "III Help Window",
		XtNwidth, 1000,
		XtNheight,1000,
		NULL);
  /*
   * Create the paned widget.
  */
    data ->frame = XtVaCreateManagedWidget ("Form",
      panedWidgetClass, data ->hpopup,
      XtNorientation, (XtOrientation) XtorientVertical,
      NULL);
  /*
   * Create the text widget.
  */
    data ->text = XtVaCreateManagedWidget ("Text",
      asciiTextWidgetClass, data->frame,
      XtNallowResize, (Boolean) True,
      XtNshowGrip, (Boolean) False,
      XtNtype, (XawAsciiType) XawAsciiFile,
      XtNstring, (String) helpfile,
      XtNscrollVertical, (XawTextScrollMode) XawtextScrollWhenNeeded,
      XtNdisplayCaret, (Boolean) False,
		NULL);	
  /*
   * Create the Done button.
  */
    data ->done = XtVaCreateManagedWidget ("Done",
      commandWidgetClass, data ->frame,
      XtNshowGrip, (Boolean) False,
      XtNskipAdjust, (Boolean) True,
      XtNlabel, (String) "Click here to dismiss",
      NULL);
    XtAddCallback (data->done, XtNcallback,
      (XtCallbackProc) HelpDoneCback, (XtPointer) data);

    XtPopup (data ->hpopup, XtGrabNone);
    XRaiseWindow(ooo ->dis, XtWindow (data->hpopup));
	 
  }
  return;
}











