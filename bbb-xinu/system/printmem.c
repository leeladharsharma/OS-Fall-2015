#include <xinu.h>

int printmem()
{
uint32 blocklt;
intmask mask; /* Saved interrupt mask */
struct memblk *prev, *curr, *addr;
mask = disable();
prev = &memlist;
curr = memlist.mnext;
//addr = NULL;

while (curr != NULL) 
{
 //addr = curr;
 blocklt = curr->mlength;
 printf("\nAddress of current block is %u", curr);
 printf("\nBlock length of current block is %u", blocklt);
 prev = curr;
 curr = curr->mnext;
 //printf("\nNew addres %u",curr);
}
restore(mask);
return 0;
}
