#include <xinu.h>
#include <myqueue.h>

/* Table of process queues	*/

 //enQueue  -  Insert a process at the tail of a queue
 
pid32 enQueue(pid32 Q[],pid32 pid, int *head, int *tail)
{		
	if(*head == -1 && *tail == -1)	{
		Q[0] = pid;
		*head = 0;
		*tail = 0;
		//return 0;
	}	
	
	if (*tail >-1 && *tail<(SIZE-1))	{
		(*tail)++;
		Q[*tail] = pid;
		//return 0;
	}
	
	return pid;
}

//deQueue  -  Remove and return the first process on a list
pid32 deQueue(pid32 Q[], int *head, int *tail)			/* ID of process removed	*/
{
	if(*head == -1 && *tail == -1)	{
		return SYSERR;
	}
	pid32 pid=Q[*head];
	if(*head >-1 && *tail > -1 && *head == *tail && *head < SIZE)	{
		pid=Q[*head];		
		Q[*tail] = -1;
		*head = -1;
		*tail = -1;
	}	
	
	if (*head >-1 && *head<SIZE)	{
		pid=Q[*head];		
		Q[*head] = -1;
		(*head)++;
	}
	return pid;
}

int isQueueEmpty(int* head, int* tail)
{
	if(*head == -1 && *tail == -1)
	{
		return 1;
	}
	return 0;
}


