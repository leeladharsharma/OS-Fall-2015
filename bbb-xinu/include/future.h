#include<prodcons.h>

#ifndef _FUTURE_H_
#define _FUTURE_H_
 
/* define states */
#define FUTURE_EMPTY	  0
#define FUTURE_WAITING 	  1         
#define FUTURE_VALID 	  2         

/* modes of operation for future*/
#define FUTURE_EXCLUSIVE  1	

typedef struct futent
{
   int *value;		
   int flag;		
   int state;         	
   pid32 pid;
} future;

extern future *f1, *f2, *f3;
typedef unsigned int uint;
uint future_prod(future *fut);
uint future_cons(future *fut);

/* Interface for system call */
future* future_alloc(int future_flags);
syscall future_free(future*);
syscall future_get(future*, int*);
syscall future_set(future*, int*);
 
#endif /* _FUTURE_H_ */
