/* xsh_udpreceiver.c - xsh_udpreceiver */

//#include <xinu.h>
#include <future.h>
#include <stdio.h>
#include <string.h>

/*------------------------------------------------------------------------
 * xsh_udpreceiver - shell command that can send a message to Linux
 *	         and receive a reply using futures
 *------------------------------------------------------------------------
 */

future *f_exclusive;

shellcmd xsh_udpreceiver(int nargs, char *args[])
{
	
	f_exclusive = future_alloc(FUTURE_EXCLUSIVE);
	
	resume( create(future_cons, 1024, 20, "fcons1", 1, f_exclusive) );
  	resume( create(future_prod, 1024, 20, "fprod1", 1, f_exclusive) );
}
