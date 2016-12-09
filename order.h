#ifndef ORDER_H
#define ORDER_H

int compareBody (const void * i, const void * j);
int compareCamera (const void * i, const void * j);
int compareTime (const void * i, const void * j);
int compareThumbnail (const void * i, const void * j);

void shitSort (void * arr, int n, int size, 
							 int (* comp) (const void * a, const void * b));
#endif
