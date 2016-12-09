#ifndef MENU_H
#define MENU_H

String ReadPageMenuItems [] = {
	"Jupiter",
	"Saturn",
	"Uranus",
	"Neptune",
	"All Planets"
};

String LayoutMenuItems [] = {
	"Across",
	"Down",
	"Peano",
	"Line on/off"
};

String OrderMenuItems [] = {
	"Initial Order",
	"By Time",
	"By Target",
	"By Camera",
	"By Thumbnail",
	"Random Order"
};

String ClusterMenuItems [] = {
	"By Time and Title"
};

void ReadPageMenuSelect(Widget w, XtPointer paneNum,  XtPointer pIII);
void OrderMenuSelect(Widget w, XtPointer paneNum,  XtPointer pIII);
void ClusterMenuSelect(Widget w, XtPointer paneNum,  XtPointer pIII);



extern String LayoutMenuItems [];
void LayoutMenuSelect(Widget w, XtPointer paneNum,  XtPointer pIII);
#endif
