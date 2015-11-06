
/* xsh_walkmem.c - xsh_walkmem */

#include <xinu.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

/*------------------------------------------------------------------------
 * xsh_mem - print mem
 *------------------------------------------------------------------------
 */

shellcmd xsh_walkmem(int nargs, char *args[]) {

        printmem();
	return 0;
}
