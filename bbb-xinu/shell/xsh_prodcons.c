//#include <prodcons.h>
#include <future.h>
#include <stdlib.h>
#include <ctype.h>



int n;                 //Definition for global variable 'n'
sid32 consumed, produced;
future *f1, *f2, *f3;
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
	 f1 = future_alloc(FUTURE_EXCLUSIVE);
	 f2 = future_alloc(FUTURE_EXCLUSIVE);
	 f3 = future_alloc(FUTURE_EXCLUSIVE);
  	resume( create(future_cons, 1024, 20, "fcons1", 1, f1) );
  	resume( create(future_prod, 1024, 20, "fprod1", 1, f1) );
  	resume( create(future_cons, 1024, 20, "fcons2", 1, f2) );
  	resume( create(future_prod, 1024, 20, "fprod2", 1, f2) );
  	resume( create(future_cons, 1024, 20, "fcons3", 1, f3) );
  	resume( create(future_prod, 1024, 20, "fprod3", 1, f3) );
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

future_free(f1);
future_free(f2);
future_free(f3);
    
}
