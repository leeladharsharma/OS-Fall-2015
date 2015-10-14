#include <future.h>

 uint future_cons(future *fut) 
{
  intmask mask;
  mask = disable();
  int i, status;
  status = future_get(fut, &i);
  if (status < 1) 
  {
    printf("future_get failed\n");
    restore(mask);
    return -1;
  }
  printf("it produced %d\n", i);
  restore(mask);
  return OK;
}
