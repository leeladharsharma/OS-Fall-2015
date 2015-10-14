
#include <future.h>
/* if future does not have value set */
syscall future_set(future *f, int *i) 
{
  if (f->state == FUTURE_EMPTY || f->state == FUTURE_WAITING)
  {
    printf("value set\n");
    f->value = (int*)*i;
    f->state = FUTURE_VALID; 
    return OK; 
  } 
  return SYSERR;
}
