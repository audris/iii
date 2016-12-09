#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <X11/Xlib.h>
#include <X11/Intrinsic.h>
#include "iii.h"

void shitSort (void * arr, int n, int size, 
							 int (* comp) (const void * a, const void * b))
{
	int i,j, where;
	void * a = malloc (size);
	arr = (char *)arr - size;
	for (j = 2; j <= n; j++){
		memcpy (a, (char *)arr + (j * size), size);
		i = j-1;
		while (i > 0 && comp (a, (char *)arr + i * size) < 0){
			memcpy ((char *)arr + (i+1) * size, (char *)arr + i * size, size);
			i--;
		}
		memcpy ((char *)arr + (i+1)*size, a, size);
	}	
	free (a);
}

int compareBody (const void * i, const void * j)
{
/*	printf ("%.5s\n", one .data .records [*(int*)i] .text .fields .body);*/
	return strncmp (
		one .data .records [*(int*)i] .text .fields .body,
		one .data .records [*(int*)j] .text .fields .body,
		sizeof (one .data .records [0] .text .fields .body));
}

int compareCamera (const void * i, const void * j)
{
	return strncmp (
		one .data .records [*(int*)i] .text .fields .instr,
		one .data .records [*(int*)j] .text .fields .instr,
		sizeof (one .data .records [0] .text .fields .instr));
}
					 
int compareTime (const void * i, const void * j)
{
	char ai [9], aj [9];
	float fi, fj;
	strncpy (ai, one .data .records [*(int*)i] .text .fields .number, 8);
	strncpy (aj, one .data .records [*(int*)j] .text .fields .number, 8);
	fi = atof (ai);
	fj = atof (aj);
	if (fi < fj)
		return -1;
	else
		if (fi > fj)
			return 1;
	return 0;
}


int compareThumbnail (const void * i, const void * j)
{
	char ai [9], aj [9];
	unsigned char icon1 [36*36], icon2 [36*36];
	int size = 36*36;
   register k;
	double sum = 0;
	memcpy (icon1, one .iconIm .images + (*(int*)i) * size +
			  one .iconIm .offset, size);
	memcpy (icon2, one .iconIm .images + (*(int*)j) * size +
			  one .iconIm .offset, size);
	for (k = 0; k < size; k++){
/*		int tmp = icon1 [k] - icon2 [k];
		int add = tmp < 0 ? -1 : 1;
		add = tmp == 0 ? 0 : add;
		sum += add;
*/
/*		sum += log ((icon1 [k]+1.0)/(icon2 [k] + 1.0));*/
		sum += icon1 [k] - icon2 [k];
	}
	if (sum < 0)
		return -1;
	if (sum > 0)
		return 1;
	return 0;
}




