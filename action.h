#ifndef ACTION_H
#define ACTION_H

XtCallbackProc LeftClickAction (Widget w, XButtonEvent * event);
XtCallbackProc ShiftLeftClickAction (Widget w, XButtonEvent * event);
XtCallbackProc CtrlLeftClickAction (Widget w, XButtonEvent * event);

XtCallbackProc MiddleClickAction (Widget w, XButtonEvent * event);
XtCallbackProc ShiftMiddleClickAction (Widget w, XButtonEvent * event);
XtCallbackProc Middle2ClickAction (Widget w, XButtonEvent * event);

XtCallbackProc RightClickAction (Widget w, XButtonEvent * event);
XtCallbackProc CtrlRightClickAction (Widget w, XButtonEvent * event);
XtCallbackProc ShiftRightClickAction (Widget w, XButtonEvent * event);
XtCallbackProc Right2ClickAction (Widget w, XButtonEvent * event);

XtCallbackProc ExposeAction (Widget w, XButtonEvent * event);
XtCallbackProc Quit (Widget w, XtPointer ooo);
XtCallbackProc Help (Widget w, XtPointer onePt);
#endif




