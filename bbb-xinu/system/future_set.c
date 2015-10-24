#include <future.h>

syscall future_set(future *f, int *value)
{
intmask mask;
  mask = disable();
  if (f->state == FUTURE_EMPTY || f->state == FUTURE_WAITING)
  {
    f->value = value;
    f->state = FUTURE_VALID; 
    resume(f->pid);
    restore(mask);
    return OK; 
  } 

  
	
	if(f->flag == FUTURE_QUEUE) 
	{
			pid32 tpid;
                        
			if(isQueueEmpty(&(f->get_head),&(f->get_tail)))
			{
				f->value=*value;
				enQueue(f->set_queue, getpid(), &(f->set_head), &(f->set_tail));
                        	suspend(getpid()); 
				return OK;
			}
			else
			{
				f->value =*value;
				tpid=deQueue(f->get_queue, &(f->get_head), &(f->get_tail));
                        	resume(f->get_queue[f->get_head]);
				return OK;
			}
			
	}
   if(f->flag == FUTURE_SHARED)
    {
    if(f->state == FUTURE_EMPTY)
    {
        pid32 tpidShared;
        f->value=*value;
        f->state = FUTURE_VALID;

        //Resume All
        while(!(isQueueEmpty(&(f->get_head),&(f->get_tail))))
        {
            tpidShared=deQueue(f->get_queue, &(f->get_head), &(f->get_tail));
            resume(f->get_queue[f->get_head]);
        }
    }
    if(f->state == FUTURE_VALID)
    {
        return OK;
    }
     return OK;
    }
restore(mask);	
return SYSERR;
}
