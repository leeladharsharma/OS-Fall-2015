//#include <prodcons.h>
#include <future.h>
#include <stdlib.h>
#include <ctype.h>



int n;                 //Definition for global variable 'n'
sid32 consumed, produced;
future *f_exclusive, *f_shared, *f_queue;
shellcmd xsh_prodcons(int nargs, char *args[])
{
      //Argument verifications and validations

     int count = 2000;             //local varible to hold count
     
 /*Initialise semaphores*/
      consumed = semcreate(1);      
      produced = semcreate(0);

      //check args[1] if present assign value to count
      int i=0,k=0;
	
      if(nargs>2)
		{
 		printf("Too many arguments\n");	
		return 1;
		}
      else if(nargs==2)
      {
	if(strcmp(args[1], "-f")==0)
 	{
 
        f_exclusive = future_alloc(FUTURE_EXCLUSIVE);
        f_shared = future_alloc(FUTURE_SHARED);
        f_queue = future_alloc(FUTURE_QUEUE);	


	// Test FUTURE_QUEUE
	resume( create(future_cons, 1024, 20, "fcons6", 1, f_queue) );
  	resume( create(future_cons, 1024, 20, "fcons7", 1, f_queue) );
  	resume( create(future_cons, 1024, 20, "fcons7", 1, f_queue) );
  	resume( create(future_cons, 1024, 20, "fcons7", 1, f_queue) );
  	resume( create(future_prod, 1024, 20, "fprod3", 1, f_queue) );
  	resume( create(future_prod, 1024, 20, "fprod4", 1, f_queue) );
  	resume( create(future_prod, 1024, 20, "fprod5", 1, f_queue) );
  	resume( create(future_prod, 1024, 20, "fprod6", 1, f_queue) );

	// Test FUTURE_EXCLUSIVE
  	resume( create(future_cons, 1024, 20, "fcons1", 1, f_exclusive));
  	resume( create(future_prod, 1024, 20, "fprod1", 1, f_exclusive));

	// Test FUTURE_SHARED
  	resume( create(future_cons, 1024, 20, "fcons2", 1, f_shared) );
  	resume( create(future_cons, 1024, 20, "fcons3", 1, f_shared) );
  	resume( create(future_cons, 1024, 20, "fcons4", 1, f_shared) ); 
  	resume( create(future_cons, 1024, 20, "fcons5", 1, f_shared) );
  	resume( create(future_prod, 1024, 20, "fprod2", 1, f_shared) );
	return 0;
	}

	if(strcmp(args[1],"--help")== 0)
	{
	 printf("This command is used to solve the producer consumer problem using semaphores\n");
	 return 0;
	}

	while(i<strlen(args[1]))
                         {
                            
			    if(isdigit(args[1][i])==0)
				{	
					printf("Only positive numbers are allowed\n");
					return 0;
				}

			    else
				k++;
				i++;

			}	
       if(k==strlen(args[1]))
		count = atoi(args[1]);
      }
	else
	{
	 count = count;
	}
 
      //create the process producer and consumer and put the in ready queue.
      //Look at the definations of function create and resume in exinu/system folder for reference.      
      resume( create(producer, 1024, 20, "producer", 1, count) );
      resume( create(consumer, 1024, 20, "consumer", 1, count) );

    
}
