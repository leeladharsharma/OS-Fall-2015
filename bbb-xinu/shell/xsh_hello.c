
/* xsh_date.c - xsh_date */

#include <xinu.h>
#include <string.h>
#include <stdio.h>

/*------------------------------------------------------------------------
 * xsh_date - obtain and print the current month, day, year, and time
 *------------------------------------------------------------------------
 */
shellcmd xsh_hello(int nargs, char *args[]) {
if(nargs==2)
{
  printf("Hello %s, Welcome to the worls of Xinu!!\n", args[1])
}
else
{
 printf("Hello User, Welcome to the world of Xinu\n")  
}
	return 0;
}
