#include <prodcons.h>
#include <stdlib.h>
#include <ctype.h>



int n;                 //Definition for global variable 'n'
sid32 consumed, produced;

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

      //create the process producer and consumer and put them in ready queue.
      //Look at the definations of function create and resume in exinu/system folder for reference.      
      resume( create(producer, 1024, 20, "producer", 1, count) );
      resume( create(consumer, 1024, 20, "consumer", 1, count) );
}
