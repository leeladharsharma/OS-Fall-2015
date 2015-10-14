#include <future.h>

uint future_prod(future *fut) 
{
  intmask mask;
  mask = disable();
  int i, j;
  j = (int)fut;
  for (i=0; i<1000; i++) 
  {
    j += i;
  }
  future_set(fut, &j);
  restore(mask);
  return OK;
}

