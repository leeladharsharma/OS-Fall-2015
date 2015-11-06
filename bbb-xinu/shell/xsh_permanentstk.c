
/* xsh_permanentstk.c - xsh_permanentstk */

#include <xinu.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

/*------------------------------------------------------------------------
 * xsh_permanentstk - allocate permanent mem through stack
 *------------------------------------------------------------------------
 */
int32 globalval;
shellcmd xsh_permanentstk(int nargs, char *args[]) {
	globalval = 0;
	getstknew(10);
        //create(procname, 1024, 20, "fcons2", 0);
	return 0;
}
