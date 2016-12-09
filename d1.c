#include <stdio.h>


main ()
{

	char buf [800*800];
	fread (buf, 1, 800, stdin);
	fprintf (stdout, "P5\n800\n800\n255\n");
	fread (buf, 1, 800*800, stdin);
	fwrite (buf, 1, 800*800, stdout);
}	

	

