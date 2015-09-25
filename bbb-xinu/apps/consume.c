#include <prodcons.h>

 void consumer(int count)
 {
     int i;

     for( i=1 ; i<=count ; i++ )
	printf("The value of n is %d \n", n);
 }
