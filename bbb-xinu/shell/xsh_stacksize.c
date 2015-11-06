
/* xsh_stacksize.c - xsh_stacksize */

#include <xinu.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

/*------------------------------------------------------------------------
 * xsh_stacksize - print Hello world
 *------------------------------------------------------------------------
 */
void praddr(int count)
{
 //printf("/nStack size updating %d times", count);
}

shellcmd xsh_stacksize(int nargs, char *args[]) {

	int count = 2;
	resume( create(praddr, 1024, 20, "pradd", 1, count) );

	return 0;
}
