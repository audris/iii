#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>

#include <X11/Xlib.h>
#include <X11/Intrinsic.h>

#include "iii.h"
#include "order.h"
#include "action.h"

int LAYOUT_ROWS_OR_COLS = 40;

#define MIN(a, b) ((a) > (b) ? (b) : (a))

void LayoutMenuSelect (Widget w, XtPointer paneNum,  XtPointer pIII)
{
	int i, x, y; 
	printf ("Layout Menu item %d was selected (widget=%d, data=%d)\n",
			  (int)paneNum, w, pIII);

	waiting (&one);
	if ((int)paneNum != 3){
		/*clearScreen (one .background);*/
		if (one .typeOfLayout == 2){
			free (one .fromNtoXY);
			free (one .fromXYtoN);
		}
		one .typeOfLayout = (int)paneNum;
		switch (one .typeOfLayout){
		case 0: /* across */
			{
				int numDown = one .NObjects / LAYOUT_ROWS_OR_COLS + 1;
				int numAcross = MIN (one .NObjects, LAYOUT_ROWS_OR_COLS);
				one .height = numDown * one .iconIm .height;
				one .width = numAcross * one .iconIm .width;
			}
			break;			
		case 1: /* down */
			{
				int numDown = MIN (one .NObjects, LAYOUT_ROWS_OR_COLS);
				int numAcross = one .NObjects / LAYOUT_ROWS_OR_COLS + 1;
				one .height = numDown * one .iconIm .height;
				one .width = numAcross * one .iconIm .width;
			}
			break;			
		case 2: /* Peano */
			one .width = one .iconIm .width * 64;
			one .height = one .iconIm .height * 64;
			one .fromNtoXY = (unsigned short *)
				malloc (64*64 * sizeof (*(one .fromNtoXY)));
			one .fromXYtoN = (unsigned short *)
				malloc (64*64 * sizeof (*(one .fromXYtoN)));
			if (one .fromXYtoN == NULL || one .fromNtoXY == NULL){
				fprintf (stderr, "Can not allocate %d bytes for LAYOUT\n",
					2 * 64*64 * sizeof (*(one .fromXYtoN)));
				exit (-1);
			}
			{
				int i;
				char buf [255];
				FILE * ff;
				sprintf (buf, "%s/%s", getenv ("PLANETS_BASE"), "h64.64.array");
				if ((ff = fopen (buf, "r")) == NULL){
					fprintf (stderr, "Can not open layout file %s\n", buf);
					exit (-1);
				}
				for (i = 0; i < 64*64; i++){
					int nn, ii, jj;
					if (3 != fscanf (ff, "%d %d %d", &nn, &ii, &jj)){
						fprintf (stderr, "error reading layout file %s\n", buf);
						exit (-1);
					}
					one .fromNtoXY [nn] = (ii << 8) + jj;
					one .fromXYtoN [ii + jj * 64] = nn;
				}
				fclose (ff);
			}
			break;			
		}
		SetupMainWindow ();
		showImageArray (&one);
	}else{
		if (one .typeOfLayout == 2){
			XPoint lin [2];
			int i;
			for (i = 0; i < 64 * 64 - 1; i++){
				lin [0] .x = (one .fromNtoXY [i] >> 8) * one .iconIm .width
					+ one .iconIm .width/2;
				lin [0] .y = (one .fromNtoXY [i] & 0xff)* one .iconIm .height
					+ one .iconIm .height/2;
				lin [1] .x = (one .fromNtoXY [i+1] >> 8) * one .iconIm .width
					+ one .iconIm .width/2;
				lin [1] .y = (one .fromNtoXY [i+1] & 0xff)* one .iconIm .height
					+ one .iconIm .height/2;
				XDrawLines (one .dis, one .pixmap, one .xorgc, lin,
					2, CoordModeOrigin);
			}
		}
	}
	doneWaiting (&one);
}


void ReadPageMenuSelect (Widget w, XtPointer paneNum,  XtPointer pIII)
{
	char ind[255], hist[255], icon[255], lvr [255], cd [255], pageName [255];
	int Nimages, width=36, height=36;
	waiting (&one);
	printf ("Read Page Menu item %d was selected (widget=%d, data=%d)\n",
			  (int)paneNum, w, pIII);
	switch ((int)paneNum){
	case 5-1:
		sprintf (pageName, "all");
		Nimages = 26435;
		break;
	case 4-1:
		sprintf (pageName, "neptune");
		Nimages = 3048 + 2956 + 3005 + 831;
		break;
	case 3-1:
		sprintf (pageName, "uranus");
		Nimages = 2838 + 2551 + 1149 -1;
		break;
	case 2-1:
		sprintf (pageName, "saturn");
		Nimages = 2623 + 1402;
		break;
	case 1-1:
		sprintf (pageName, "jupiter");
		Nimages = 2548 + 2547 + 938;
		break;
	default:
		Nimages = 0;
		break;
	}
	
	if ((int)paneNum >= 0)
		sprintf (one .TheName, "%s/%s",
					 getenv ("PLANETS_BASE"), pageName);
	else
		sprintf (one .TheName, "%s", pageName);
		

	if (one .data .records){
		free (one .data .records);
	}
	if (one .NObjects){
		DestroyXImageArray (&one .iconIm);
	}
	
	one .NObjects = Nimages;
	one .data = CreateImageDataArray (one .TheName, Nimages);
	one .iconIm = CreateXImageArray (one .TheName, width, height, Nimages);
	
  	LayoutMenuSelect (0, 0, 0);
	one .AnimateFrom = one .AnimateTo = -1;
	
	doneWaiting (&one);	
}

void ClusterMenuSelect (Widget w, XtPointer paneNum,  XtPointer pIII)
{
	int i, x, y;
	int * twoD = (int*) malloc (2 * one .NObjects * sizeof (int));
	if (twoD == NULL){
		fprintf (stderr, "Can not allocate %d bytes for clustering\n",
					2 * one .NObjects * sizeof (int));
		exit (-1);
	}
			
	waiting (&one);
	printf ("Cluster Menu item %d was selected (widget=%d, data=%d)\n",
			  (int)paneNum, w, pIII);
	doneWaiting (&one);	
	free (twoD);
}


void OrderMenuSelect(Widget w, XtPointer paneNum,  XtPointer pIII)
{
	int i, x, y;
	int (*compar)(const void *, const void *);
	waiting (&one);
	printf ("Order Menu item %d was selected (widget=%d, data=%d)\n",
			  (int)paneNum, w, pIII);

	switch ((int) paneNum){
	case 0:
		for (i = 0; i < one .NObjects; i++)
			one .iconIm .order [i] = i;
		break;
	case 1:
		compar = compareTime;		
		break;
	case 2:
		compar = compareBody;		
		break;
	case 3:	
		compar = compareCamera;
		break;
	case 4:	
		compar = compareThumbnail;
		break;
	case 5:/* random*/
		{
			int * tmpOrd;
			tmpOrd = (int *) malloc (one .NObjects * sizeof (int));
			for (i = 0; i < one .NObjects; i++)
				tmpOrd [i] = one .iconIm .order [i];
			for (i = 0; i < one .NObjects; i++){
				int j, tmp = (double)(rand ()) /
								 0xffff * (one .NObjects - i);
				if (tmp >= one .NObjects - i || tmp < 0)
					fprintf (stderr, "Bad Bad %d %d %d\n", i, tmp,
								one .NObjects - i); 
				one .iconIm .order [i] = tmpOrd [tmp];
				for (j = tmp; j < one .NObjects - i - 1; j++)
					 tmpOrd [j] = tmpOrd [j+1];
			}
			free (tmpOrd);
		}
		break;
	}
	if ((int) paneNum && (int) paneNum != 5){
		shitSort ((void *)one .iconIm .order, one .NObjects,
				 sizeof (one .iconIm .order [0]), compar);
/*		for (i = 0; i < one .NObjects; i++)
			printf ("%d %d %.19s-%.8s-%.39s\n",
					i, one .iconIm .order [i],  
			one .data .records [one .iconIm .order [i]] .text .fields .instr,
			one .data .records [one .iconIm .order [i]] .text .fields .body,
			one .data .records [one .iconIm .order [i]] .text .fields .direct					  );
*/					  
	}
	for (i = 0; i < one .NObjects; i++)
		one .iconIm .invOrder [one .iconIm .order [i]] = i;
	showImageArray (&one);
	doneWaiting (&one);
}




