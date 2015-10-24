#include <future.h>

syscall future_get(future *f, int* value)
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
	if(f->state ==FUTURE_EMPTY && f->flag == FUTURE_QUEUE)
	{
		//Add it to the get queue
		intmask mask = disable();

		//SET EMPTY
		if(isQueueEmpty(&(f->set_head),&(f->set_tail)))
		{
			enQueue(f->get_queue,getpid(),&(f->get_head),&(f->get_tail));
			suspend(getpid());
			*value = f->value;	
		}
		else
		{
			pid32 tpid;			
			tpid=deQueue(f->set_queue, &(f->set_head), &(f->set_tail));
                        resume(tpid);
			*value = f->value;	
			return OK;
		}

		restore(mask);
		return OK;
	}
	
if(f->flag == FUTURE_SHARED)
    {

      if(f->state == FUTURE_EMPTY)
      {
          enQueue(f->get_queue,getpid(),&(f->get_head),&(f->get_tail));      // enqueue
          suspend(getpid());
      //return OK;
      }
      if(f->state == FUTURE_VALID)
      {
          *value = f->value;    
          return OK;
      }

    }
	
return SYSERR;
}
