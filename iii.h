#define MAP_FILE 0
typedef struct
{
	union {
		char buff [336];
		struct {
			char     name[9];   
		   char     phase[17]; /*9*/
		   char     body[8];   /*26*/
		   char     id[10];    /*34*/
		   char     number[8]; /*44*/
		   char     itime[20]; /*52*/
		   char     etime[20]; /*72*/
		   char     instr[19]; /*92*/
		   char     scanr[7];  /*111*/
		   char     shutt[7];  /*118*/
		   char     gain[7];   /*125*/
		   char     edit[7];   /*132*/
		   char     filter[7]; /*139*/
		   char     filtid[4]; /*146*/
		   char     expos[7];  /*150*/
		   char     note[80];  /*157*/
		   char     bitmas[8]; /*237*/
		   char     anomal[6]; /*245*/
		   char     volnum[8]; /*251*/
		   char     direct[31];/*259*/
		   char     brovol[8]; /*290*/
		   char     dirbr[38]; /*298*/
		} fields;
	} text;
/*	unsigned short histo [256]; */
} Data;

typedef struct {
	Data * records;
} ImageDataArray; 


typedef struct {
	int NImages, width, height;
	unsigned int * order;
	unsigned int * invOrder;
	XImage template;
	int offset;
	char * data;
	FILE * pgmFile;
	unsigned char * images;
	char fromFile;
} XImageArray; 


typedef struct { 
	XtAppContext appCon;
	Widget     toplevel, app, view, vpane, mainW, bb,
					out, help, scroll, vertical, horizontal, form;
	Widget readPageButton, readPage,
		layoutButton, layout,
		orderButton, order,
		clusterButton, cluster;
	Display * dis;
	int screen, depth, foreground, background;
	Window rwin;
	Pixmap pixmap;
	Colormap cmap;
	XColor * color;
	XGCValues val;
	
	Visual * vis;

	GC textgc, xorgc, xorOutlinegc, 
	currentgc, highlightgc, 
	stip1gc, bitgc;
	char * DisplayName;
	Colormap defCmap;
	int nOfEntries;
	Pixmap stiple;
	int width;
	int height;
	Visual * directVis, * trueVis;
	XSetWindowAttributes attr;
	XFontStruct *fontInfo;

	Cursor Wait, Cross;

	int top, left;
	XImage * im;
	XImage * fullImage;

	int NSelected;
	unsigned short int * SelectedIndex;
	
	int ThePage;
	char TheName [255];
	int NObjects;
	unsigned short int * CDindex,
	    * LVRindex;
	XImageArray iconIm;
	ImageDataArray data;
	int typeOfLayout;
	unsigned short int * fromXYtoN;
	unsigned short int * fromNtoXY;
	int AnimateFrom, AnimateTo;
} iii;

extern iii one;

ImageDataArray CreateImageDataArray (char * indxName, int NImages);
XImageArray CreateXImageArray (char * fName, int width, int height,
										 int NImages);
void DestroyXImageArray (XImageArray * ans);

void initGraph ();
void closeGraph (iii * ooo);
void clearScreen (int col);

void fillLine
	(
	 int col,     /* Color */
	 int length,  /* Number of points in the line
						  (half the length of the array line */
	 short * line /* the array of points x1,y1,x2,y2,... */
	 );
void drawLine
	(
	 int col,     /* Color */
	 int length,  /* Number of points in the line
						  (half the length of the array line */
	 short * line /* the array of points x1,y1,x2,y2,... */
	 );
void drawPoints
	(
	 int col,     /* Color */
	 int length,  /* Number of points in the line
						  (half the length of the array line */
	 short * line /* the array of points x1,y1,x2,y2,... */
	 );
void text
	(
	 int x, /* horizontal offset */
	 int y, /* vertical offset   */
	 char * str /* text itself */
	 );
void verText (int x, int y, float rotation, int allignment, char * str);
void magText (float mag);
void setTextAtt
	(
	 int color, /* color of the text */
	 char * font /* Name of the X font (as shown by xlsfonts) */
	 );

void flushGraph (void);
XImage * CreateXImage (int width, int height);
void waiting (iii *);
void doneWaiting (iii *);

#ifdef  __cplusplus	
};
#endif	









