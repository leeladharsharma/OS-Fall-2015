//#include <xinu.h>
#include <future.h>
/* if future does not have value set */
syscall future_set(future *f, int *i) 
{
  intmask mask;
  mask = disable();
  if (f->state == FUTURE_EMPTY || f->state == FUTURE_WAITING)
  {
    f->value = i;
    f->state = FUTURE_VALID; 
    resume(f->pid);
    restore(mask);
    return OK; 
  } 
  restore(mask);
  return SYSERR;
}
