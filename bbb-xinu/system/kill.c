/* kill.c - kill */

#include <xinu.h>

/*------------------------------------------------------------------------
 *  kill  -  Kill a process and remove it from the system
 *------------------------------------------------------------------------
 */
syscall	kill(
	  pid32		pid		/* ID of process to kill	*/
	)
{
	intmask	mask;			/* Saved interrupt mask		*/
	struct	procent *prptr;		/* Ptr to process' table entry	*/
	int32	i;			/* Index into descriptors	*/
	int32 gval;
	gval = globalval;
	uint32 k;
	uint32 *stbase;
	int32 counter;
	mask = disable();
	if (isbadpid(pid) || (pid == NULLPROC)
	    || ((prptr = &proctab[pid])->prstate) == PR_FREE) {
		restore(mask);
		return SYSERR;
	}

	if (--prcount <= 1) {		/* Last user process completes	*/
		xdone();
	}

	send(prptr->prparent, pid);
	for (i=0; i<3; i++) {
		close(prptr->prdesc[i]);
	}

	if(strcmp(prptr->prname, "pradd")==0)
	{
	 //printf("\nInside my process");
	 stbase = (uint32 *)prptr->prstkbase;
	
	for(k=0; k < (prptr->prstklen); k++)
	{
	 if(*--stbase != 'f')
	  {
	   counter= counter +1 ;
	  }
	 else
	  {
	 
		continue;	
	  }
	}
	}
	printf("\nStack size for process is %d", counter);
	if(gval == 0)
	{
	 printf("normal path");
	 freestk(prptr->prstkbase, prptr->prstklen);
	}

	
	switch (prptr->prstate) {
	case PR_CURR:
		prptr->prstate = PR_FREE;	/* Suicide */
		resched();

	case PR_SLEEP:
	case PR_RECTIM:
		unsleep(pid);
		prptr->prstate = PR_FREE;
		break;

	case PR_WAIT:
		semtab[prptr->prsem].scount++;
		/* Fall through */

	case PR_READY:
		getitem(pid);		/* Remove from queue */
		/* Fall through */

	default:
		prptr->prstate = PR_FREE;
	}

	restore(mask);
	return OK;
}
