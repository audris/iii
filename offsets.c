#include <stdio.h>


int getOffset (int thePage, int * ii)
{
	int offset;
	switch (thePage){
	case 5:
		if (*ii < 6033){
			offset = 5329;
			break;
		}
		if (*ii < 10058){
			offset = 1304;
			*ii -= 6033;
			break;
		}
		if (*ii < 16595){
			offset = 21202;
			*ii -= 10058;
		break;
		}
		offset = 11362;
		*ii -= 16595;
		break;
	case 4:
		offset = 11362;
		break;
	case 3:
		offset = 21202;
		break;
	case 2:
		offset = 1304;
		break;
	case 1:
		offset = 5329;
		break;
	default:
		offset = 0;
	}
	return offset;
}

main (int argc, char * argv [])
{
	int page = atoi (argv[1]);
	int i, Nimages, CDoffset;
	switch (page){
		case 5:
/*		sprintf (pageName, "all");*/
		Nimages = 26435;
		CDoffset = 0;
		break;
	case 4:
/*		sprintf (pageName, "neptune");*/
		Nimages = 3048 + 2956 + 3005 + 831;
		CDoffset = 26435 - Nimages;
		break;
	case 3:
/*		sprintf (pageName, "uranus");*/
		Nimages = 2838 + 2551 + 1149 -1;
		CDoffset = 0;
		break;
	case 2:
/*		sprintf (pageName, "saturn");*/
		Nimages = 2623 + 1402;
		CDoffset = 2838 + 2551 + 1149 -1;
		break;
	case 1:
/*		sprintf (pageName, "jupiter");*/
		Nimages = 2548 + 2547 + 938;
		CDoffset = 2838 + 2551 + 1149 -1 + 2623 + 1402;
		break;
	default:
		Nimages = 0;
		break;
	}

/* LVR offsets */
	for (i = 0; i < Nimages; i++){
		int ii = i;
		int offset = getOffset (page, &ii);
		fprintf (stdout, "%d\n", ii + offset);
	}

/* CD offsets */
/*	for (i = 0; i < Nimages; i++){
		int ii = i;
		fprintf (stdout, "%d\n", i + CDoffset);
	}
*/	
}





