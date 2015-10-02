#include <prodcons.h>

 void consumer(int count)
{
     int i;

     for( i=1 ; i<=count ; i++ )
{	wait(produced);
	printf("The value consumed is %d \n", n);
	signal(consumed);
}

semdelete(consumed);
}
 
