
/* xsh_permanentmem.c - xsh_permanentstk */

#include <xinu.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

/*------------------------------------------------------------------------
 * xsh_permanentmem - allocate permanent mem through heap
 *------------------------------------------------------------------------
 */
int32 globalval;
shellcmd xsh_permanentmem(int nargs, char *args[]) {
	globalval = 0;
        printf("Mem allocate calling");
        char *blkadd = getmemnew(10);
	printf("\nblk addr %s",blkadd);
	freemem(*blkadd,10);
	return 0;
}
