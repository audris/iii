#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <X11/Xlib.h>
#include <X11/cursorfont.h>
#include <X11/Intrinsic.h> 
#include <X11/Xaw/Simple.h>
#include "iii.h"


void waiting (iii * ooo)
{
	XtVaSetValues (ooo ->mainW,  XtNcursor, ooo ->Wait, 0);
}

void doneWaiting (iii * ooo)
{
	XtVaSetValues (ooo ->mainW,  XtNcursor, ooo ->Cross, 0);
}

unsigned int pgmPars (char * fName, int * ppmWid, int * ppmHei)
{
	FILE * in = fopen (fName, "r");
	unsigned char * bitmap;
	int tmp, offset;
	if (in == NULL){
		fprintf (stderr, "can not open file %s\n",
					fName);
		exit (-1);
	}
	if (fgetc (in) != 'P' || fgetc (in) != '5'){
		fprintf (stderr, "Wrong (not pgm) file fomat");
		exit (-1);
	}
	if (3 != fscanf (in, "%d %d %d", ppmWid, ppmHei, &tmp)){
		fprintf (stderr, "Error reading pgm file");
		exit (-1);
	}
	if (10 != fgetc (in)){
		fprintf (stderr, "Error reading pgm file: no newline\n");
		exit (-1);
	}
	
/*	if ((bitmap = (unsigned char *)malloc (*ppmWid * *ppmHei *
													  sizeof (*bitmap))) == NULL){
		fprintf (stderr, "Not enough memory (need %d bytes)\n", *ppmWid * *ppmHei);
		exit (-1);
	}

	fread (bitmap, *ppmWid * *ppmHei, sizeof (unsigned char), in);
*/
	offset = ftell (in);
	fclose (in);
	return offset;
}

XImageArray CreateXImageArray (char * fName, int width, int height, int NImages)
{
	XImageArray ans;
	unsigned char * bitmap;
	XImage examp, * tmp;
	int ppmWid, ppmHei, i, j, k;
	if (NImages == 0){
		return ans;
	}
	ans .fromFile = 0;
	ans .NImages = NImages;
	ans .width = width;
	ans .height = height;
	ans .order = (unsigned int *) malloc (NImages * sizeof (*(ans .order)));
	ans .invOrder = (unsigned int *) malloc (NImages *
														  sizeof (*(ans .invOrder)));
	if (ans .order == NULL || ans .invOrder == NULL){
		fprintf (stderr, "can not allocate %d images\n",
					fName, NImages);
		exit (-1);
	}
	tmp = CreateXImage (width, height);
	ans .template = * tmp;
	ans .data = (char *)malloc (width * height * sizeof (*(ans .data)));
	ans .template .data = ans .data;
	XDestroyImage (tmp);
	{
		char buf [1000];
		int trash;
		char * temp;
		sprintf (buf, "%s.pgm", fName); 
		ans .offset = pgmPars (buf, &trash, &trash);
		ans .pgmFile = fopen (buf, "r");
		temp = (char *) mmap (0, NImages * width * height,
								  PROT_READ, MAP_FILE | MAP_SHARED,
								  fileno (ans .pgmFile), 0);
		if ((int)temp != -1){
			ans .images = (unsigned char *) temp + ans .offset;
			fclose (ans .pgmFile);
			ans .pgmFile = 0;
			fprintf (stdout, "mmap of %d bytes at %p, %d\n",
						NImages * width * height,
						temp, temp);
		}else{
			fprintf (stdout, "mmap of %d bytes unsuccesfull %d\n",
						NImages * width * height, temp);
			ans .images = 0;
			ans .fromFile = 1;
		}
	}
	for (k = 0 ; k < NImages; k++){		
		ans .order [k] = k;
		ans .invOrder [k] = k;
	}
	fprintf (stderr, "Exiting from CreateImageArray\n");
	return ans;
}	

void DestroyXImageArray (XImageArray * ans)
{
	free (ans ->order);
	free (ans ->invOrder);
	free (ans ->data);
	if (ans ->fromFile)
		fclose (ans ->pgmFile);
	else
		munmap ((char *)ans ->images - ans ->offset, ans ->NImages * ans ->width *
			  ans ->height);
}	

ImageDataArray CreateImageDataArray (char * indxName, int NImages)
{
	FILE * idx;
	ImageDataArray ans;
	int i;
	one .NSelected = 0;
	one .SelectedIndex = (unsigned short int *) malloc (NImages *
													 sizeof (*(one .SelectedIndex)));
	
	fprintf (stderr, "in CreateImageArray %s %d\n", indxName, NImages);	
	{
		char buf [1000];
		sprintf (buf, "%s.cd", indxName); 
		idx = fopen (buf, "r");
		one .CDindex = (unsigned short int *) malloc (NImages *
													 sizeof (*(one .CDindex)));
		if (idx == NULL || one .CDindex == NULL){
			fprintf (stderr, "can not open file %s or allocate %d bytes\n",
					buf, NImages * sizeof (*(one .CDindex)));
			exit (-1);
		}
		for (i = 0; i < NImages; i++){
			fscanf (idx, "%hu", one .CDindex + i);
		}
						
		fclose (idx);			

		
		sprintf (buf, "%s.lvr", indxName); 
		idx = fopen (buf, "r");
		one .LVRindex = (unsigned short int *) malloc (NImages *
													  sizeof (*(one .LVRindex)));
		if (idx == NULL || one .LVRindex == NULL){
			fprintf (stderr, "can not open file %s or allocate %d bytes\n",
					buf, NImages * sizeof (*(one .LVRindex)));
			exit (-1);
		}
		for (i = 0; i < NImages; i++){
			fscanf (idx, "%hu", one .LVRindex + i);
/*			fprintf (stderr, "%hu\n", one .LVRindex [i]);*/
		}
		fclose (idx);			

		
		sprintf (buf, "%s.index", indxName); 
		idx = fopen (buf, "r");

		if (idx == NULL){
			fprintf (stderr, "can not open file %s\n",
					buf);
			exit (-1);
		}
	}
	ans .records = (Data *) malloc (NImages * sizeof (Data));
	if (ans .records == NULL){
		fprintf (stderr, "Can not allocate %d bytes\n", NImages * sizeof (Data));
		exit (-1);
	}
	for (i = 0; i < NImages; i++){
		int j;
		char buff [200];
		fread (ans .records [i] .text .buff,
				 sizeof (ans .records [i] .text .buff), 1, idx);
/*		for (j = 0; j < 256; j++){
			int tmp;
			fscanf (his, "%d", &tmp);
			ans .records [i] .histo [j] = tmp;
		}
*/	}
	fclose (idx);
	fprintf (stderr, "Exiting from CreateImageDataArray\n");
	return ans;
}


#include <signal.h>
#include <sys/signal.h>
static void svHandler (int sig)
{
	if (sig == 0){		
		if (0!=signal (2, svHandler)){
			printf ("could not install signal handler 2\n");
		}
		if (0!=signal (3, svHandler)){
			printf ("could not install signal handler 3\n");
		}
		if (0!=signal (15, svHandler)){
			printf ("could not install signal handler 15\n");
		}
	}else{
		if (0!=signal (sig, svHandler)){
			printf ("could not install signal handler %d\n", sig);
		}
		printf ("in signal %d\n", sig);
	}
}

void selectRecords (char * fname, char * fromFname, int NItems, int * index,
						char * ext, int size)
{
	FILE * from, * to;
	char bufF [1000], bufT [1000], *buf;
	int i, j;
	fprintf (stderr, "In selectRecords, fname=%s fromFname=%s %s %d\n",
				fname, fromFname, ext, NItems);

	sprintf (bufT, "%s.%s", fname, ext);	
	sprintf (bufF, "%s.%s", fromFname, ext);
	from = fopen (bufF, "r");
	to   = fopen (bufT, "w");
	buf = (char *) malloc (size); 
	if (from == NULL || to == NULL || buf == NULL){
		fprintf (stderr, "Can not open %s - %p or %s - %p or allocate %d bytes\n",
					bufF, bufT, size, from, to);
		exit (-1);
	}
	if (size){/* not an integer text file*/
		for (i = 0; i < NItems; i++){
			fseek  (from, size * index [i], SEEK_SET);
			fread  (buf, size, 1, from);
			fwrite (buf, size, 1, to);
		}
	}else{
		int tmp = 0;
		for (j = 0; j < index [0]; j++){
			fscanf (from, "%d", &tmp);
/*			fprintf (stderr, "%d %d\n", tmp, index [0]);*/
		}
		fscanf  (from, "%d", &tmp);
		fprintf (to,   "%d\n", tmp);
		
		for (i = 1; i < NItems; i++){
			for (j = 0; j < index [i] - index [i-1] - 1; j++)
				fscanf (from, "%d", &tmp);
			fscanf (from, "%d", &tmp);
			fprintf (to, "%d\n", tmp);
		}
	}
	free (buf);
	fclose (from);
	fclose (to);
	fprintf (stderr, "Exiting from selectRecords\n");
}
			
				
	

void selectToFile (char * fname, char * fromFname, int NItems, int * index)
{
	FILE * pgm, * pgmFrom;
	char  iconFrom [255], icon [255];
	int i, offset;
	
	fprintf (stderr, "In selectToFile, fname=%s fromFname=%s\n", fname, fromFname);

	selectRecords (fname, fromFname, NItems, index,
						"index", sizeof (one .data .records ->text));
	selectRecords (fname, fromFname, NItems, index,
						"lvr", 0);
	selectRecords (fname, fromFname, NItems, index,
						"cd", 0);

/* now select pgm records */	
	sprintf (icon, "%s.pgm", fname);
	sprintf (iconFrom, "%s.pgm", fromFname);

	offset = pgmPars (iconFrom, &i, &i);
	pgmFrom = fopen (iconFrom, "r");
	pgm = fopen (icon, "w");
	if (pgmFrom == NULL || pgm == NULL){
		fprintf (stderr, "Can not open %s - %p or %s - %p\n", iconFrom,
					pgmFrom, icon, pgm);
		exit (-1);
	}
	fprintf (pgm, "P5\n%d %d\n255\n", 36, 36 * NItems); 
	for (i = 0; i < NItems; i++){
		unsigned char buf [36 * 36];
		fseek (pgmFrom, 36 * 36 * index [i] + offset, SEEK_SET);
		fread (buf, 36 * 36, 1, pgmFrom);
		fwrite (buf, 36 * 36, 1, pgm);
	}
	fclose (pgm);
	fclose (pgmFrom);		
	fprintf (stderr, "Exiting from selectToFile\n");
}


void getIndex (char * fname, char * fromFname)
{
	FILE * ind = fopen (fname, "r"); 
	int i=0, j, * selectInd;
	char buf [255];
	i = 0;
	while (!feof (ind)){
		if (1 != fscanf (ind, "%d", &j))
			break;
		i++;
	}
	one .NObjects = i;
	fseek (ind, 0, SEEK_SET);
	selectInd = (int *) malloc (one .NObjects *
													  sizeof (*(selectInd)));										  
	if (selectInd == NULL){
		fprintf (stderr, "Can not allocate open index array %d\n",
					one .NObjects);
		exit (-1);
	}
	for (i = 0; i < one .NObjects; i++){
		int ii;
		fscanf (ind, "%d", selectInd + i);
	}		
	fclose (ind);

	fprintf (stderr, "In getIndex, fname=%s size=%d\n", fname, one .NObjects);

	tmpnam (buf);
	strcpy (one .TheName, buf);
	selectToFile (one .TheName, fromFname, one .NObjects, selectInd);
	free (selectInd);
												 
	
	one .iconIm = CreateXImageArray (one .TheName, 36, 36, one .NObjects);
	one .data = CreateImageDataArray (one .TheName, one .NObjects);
	LayoutMenuSelect (0, 0, 0);
	one .AnimateFrom = one .AnimateTo = -1;
}

void getAll (char * fromFname)
{

/* get the number of objects */	
  	FILE * ind;
	int i=0;
	char buf [255];
	sprintf (buf, "%s.cd", fromFname);
	ind = fopen (buf, "r");
	if (ind == NULL){
		fprintf (stderr, "Can not open file %s\n", buf);
		exit (-1);
	}
	i = 0;
	while (!feof (ind)){
		int j;
		if (1 != fscanf (ind, "%d", &j))
			break;
		i++;
	}
	one .NObjects = i;
	fclose (ind);


/* start iii */	
	strcpy (one .TheName, fromFname);
 	one .iconIm = CreateXImageArray (one .TheName, 36, 36, one .NObjects);
	one .data = CreateImageDataArray (one .TheName, one .NObjects);
	LayoutMenuSelect (0, 0, 0);
	one .AnimateFrom = one .AnimateTo = -1;
}

int main (int argc, char * argv [])
{
	int i;
	char buf [255];
	Cursor Wait, Cross;
	svHandler (0);
  
	initGraph ();

	switch (argc){
	case 3:
		getIndex (argv [1], argv [2]);
		break;
	case 2:
		getAll (argv [1]);
		break;
	default:
		break;
	}
	XtAppMainLoop (one .appCon);	

	{
		XEvent event;
		int i = 0;
		XSelectInput(one .dis, DefaultRootWindow(one .dis), PropertyChangeMask);
		while (1){
			XtAppNextEvent(one .appCon, &event);
		  	if (event.type == ButtonPress &&
				 event .xbutton .button == Button1) {
				printf ("%d\n", i);
				/*clearScreen (i);*/
				flushGraph ();
			}else{
				XtDispatchEvent(&event);
			}
		}
	}
}




