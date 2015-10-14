//#include <xinu.h>
#include <future.h>
future* future_alloc(int future_flags)
{
  register future* f;
  f = (future*)getmem(sizeof(future));
if(f != SYSERR)
{
  f->flag= future_flags;
  f->state = FUTURE_EMPTY;
  return f;
}
 return SYSERR;
}

