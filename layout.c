#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>

#include <X11/Xlib.h>
#include <X11/Intrinsic.h>

#include "iii.h"
#include "layout.h"

int fromXYtoN (iii * ooo, int x, int y)
{
	int hNum = ooo ->width / ooo ->iconIm .width,
			 vNum = ooo ->height / ooo ->iconIm .height;
	int tmp, ans;
	switch (ooo ->typeOfLayout){
	case 0:
		tmp = y / ooo ->iconIm .height; 
		ans = x / ooo ->iconIm .width + tmp * hNum;  
		break;
	case 1:
		tmp = x / ooo ->iconIm .width;
		ans = y / ooo ->iconIm .height + tmp * vNum;
		break;
	case 2:
		tmp = x / ooo ->iconIm .width;
		ans = y / ooo ->iconIm .height;
		ans = ooo ->fromXYtoN [tmp + ans * hNum];
	}
	return ans;
}

void fromNtoXY (iii * ooo, int n, int * x, int * y)
{
	int hNum = ooo ->width / ooo ->iconIm .width,
			 vNum = ooo ->height / ooo ->iconIm .height;
	int t1, t2;
	double sqrt (double);
	switch (ooo ->typeOfLayout){
	case 0:
		*x = ((int)(n % hNum)) * ooo ->iconIm .width;
		*y = ((int)(n / hNum)) * ooo ->iconIm .height;
		break;
	case 1:
		*x = ((int)(n / vNum)) * ooo ->iconIm .width;
		*y = ((int)(n % vNum)) * ooo ->iconIm .height;
		break;
	case 2:
		{
			unsigned short num =  ooo ->fromNtoXY [n];
			*x = (num >> 8)  * ooo ->iconIm .width;
			*y = (num & 0xff)  * ooo ->iconIm .height;
		}
	}	
}

void showImageArray (iii * one)
{
	int i, j, k, x, y;
	int size = one ->iconIm .width * one ->iconIm .height;
	clearScreen (one ->background);
	for (i = 0; i < one ->NObjects; i++){
		int which = one ->iconIm .invOrder [i];
		fromNtoXY (one, which,  &x, &y);
		if (x <= one ->width - one ->iconIm .width &&
			 y <= one ->height -one ->iconIm .height){
			if (one ->iconIm .fromFile){
				fseek (one ->iconIm .pgmFile, 
					i * size
						 + one ->iconIm .offset, SEEK_SET);
				fread (one ->iconIm .data, size,
						 1, one ->iconIm .pgmFile);
			}else{
				one ->iconIm .template .data =
					one ->iconIm .images + i * size;
			}
			XPutImage (one ->dis, one ->pixmap, one ->currentgc,
						  &(one ->iconIm .template),
						  0, 0, x, y, 
						  one ->iconIm .width, one ->iconIm .height);
/*			printf ("%.8s %d %d %d %d\n",
					  one ->data .records [i]
					  .text .fields .body,
					  one ->iconIm .invOrder [i], i, x, y);
*/		}
	}
	if (!one ->iconIm .fromFile)
		one ->iconIm .template .data = one ->iconIm .data;
	for (i = 0; i < one ->NSelected; i++){
		int which = one ->iconIm .invOrder [one ->SelectedIndex [i]];
		fromNtoXY (one, which,  &x, &y);
		if (x <= one ->width - one ->iconIm .width &&
			 y <= one ->height -one ->iconIm .height){
			XPoint lin [5];
			lin [0] .x = x + 1; 
			lin [0] .y = y + 1; 
			lin [1] .x = x + one ->iconIm .width - 1; 
			lin [1] .y = y + 1; 
			lin [2] .x = x + one ->iconIm .width - 1; 
			lin [2] .y = y + one ->iconIm .height - 1; 
			lin [3] .x = x + 1; 
			lin [3] .y = y + one ->iconIm .height - 1; 
			lin [4] .x = x + 1; 
			lin [4] .y = y + 1;
			XDrawLines (one ->dis, one ->pixmap, one ->xorOutlinegc, lin,
					5, CoordModeOrigin);
		}
	}		
	flushGraph ();
}
