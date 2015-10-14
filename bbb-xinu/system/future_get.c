
#include <future.h>
/* if future has value return */
syscall future_get(future *f, int *value)
{

intmask mask;

if(f->state == FUTURE_EMPTY || f->state == FUTURE_EXCLUSIVE)
{
	
	f->state = FUTURE_WAITING;
	f->pid = getpid();
        mask = disable();
	suspend(f->pid);
	
		if (f->state == FUTURE_VALID)
		{
		*value = *(f->value);
	        f->state = FUTURE_EMPTY;
		restore(mask);
		return OK;
		}
	
}
if(f->state == FUTURE_EMPTY || f->state == FUTURE_EXCLUSIVE)
{
 
  restore(mask);
  return SYSERR;
}
}
