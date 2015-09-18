/************************************************************************/
/*		Submitted by : Aritra De and Leeladhar Sharma		*/
/*		Assignment 1 | Advanced Operating Systems		*/
/*		Submitted On : 17.09.2015				*/
/************************************************************************/
/* xsh_hello.c - xsh_hello */

#include <xinu.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

/*------------------------------------------------------------------------
 * xsh_hello - print Hello world
 *------------------------------------------------------------------------
 */

shellcmd xsh_hello(int nargs, char *args[]) {
int i=0,k=0;
	if(nargs==2)
		{       
			while(i<strlen(args[1]))
                         {
                            
			    if(isalpha(args[1][i])==0)
				{	
					printf("Special characters and digits not allowed\n");
					break;
				}

			    else
				k++;
				i++;

			}	
	if(k==strlen(args[1]))
		printf("Hello %s, Welcome to the world of Xinu!!\n", args[1]);
		
  			
		}
	else if(nargs>2)
		{
 			printf("Too many arguments\n");	
		}
	else
		{
			 printf("Too little arguments\n");
		}	

	return 0;
}
