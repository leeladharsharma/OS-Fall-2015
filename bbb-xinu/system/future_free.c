//#include <xinu.h>
#include <future.h>
/* sets state of future to FREE */
syscall future_free(future *f)
{
  return freemem(f, sizeof(f));
}
