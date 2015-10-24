
#include <future.h>
future* future_alloc(int future_flags)
{
  register future* f;
  f = (future*)getmem(sizeof(future));

if(f != SYSERR)
{
 if(f->flag == FUTURE_QUEUE)
{
    	f->set_head = -1;
	f->set_tail= -1;
	f->get_head = -1;
	f->get_tail = -1;
}
  f->flag= future_flags;
  f->state = FUTURE_EMPTY;

  return f;
}

 return SYSERR;
}

