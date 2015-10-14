
#include <future.h>
/* if future has value return */
syscall future_get(future *f, int *value)
{

if(f->state == FUTURE_EMPTY || f->state == FUTURE_EXCLUSIVE)
{
	
	f->state = FUTURE_WAITING;
	f->pid = getpid();
        intmask mask;
        mask = disable();

	while(TRUE)
	{       
		printf("Inside while");
		if (f->state == FUTURE_VALID)
		{
		*value = f->value;
	        f->state = FUTURE_EMPTY;
		restore(mask);
		return OK;
		}
	}
}
if(f->state == FUTURE_EMPTY || f->state == FUTURE_EXCLUSIVE)
{
  intmask mask;
  mask = disable();
  kprintf("");
  restore(mask);
  return SYSERR;
}
}
