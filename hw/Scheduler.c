#include "Init.h"
#include "mine.h"
#include "Thread.h"
#include "Scheduler.h"
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#define TIMESLICE	(2)


int		RunScheduler( void )
{
	Thread* runThread=NULL;
	while(1)
	{
		if(ReadyQHead!=NULL)
		{
			//printf("in\n");
			runThread=ReadyQHead;
			//printf("switch\n");
			
			__ContextSwitch(runThread,ReadyQHead->pNext);
			
			//printf("switch end\n");
			sleep(TIMESLICE);
		}
		else
		{
			runThread=ReadyQHead;
			__thread_wakeup(runThread);
			sleep(TIMESLICE);
		}
	}
}


void            __ContextSwitch(Thread* pCurThread, Thread* pNewThread)
{
	__thread_wakeup(pCurThread);
	//printf("%u  ,   %u\n", pCurThread, pNewThread);
	if(pNewThread == NULL)
		return;	// no next
	
	pCurThread->status=THREAD_STATUS_READY;
	pCurThread->bRunnable=TRUE;
	pthread_kill(pCurThread->tid, SIGUSR1); // sleep running th
	
	pCurThread->status = THREAD_STATUS_RUN;	// running next th
	
	//printf("%u hey wake up!!\n", (unsigned int)pthread_self());
	//__thread_wakeup(pCurThread);
	//printf("%u hey wake up??\n", (unsigned int)pthread_self());
	ReadyQHead=pCurThread->pNext;
	if(ReadyQHead == NULL)
		ReadyQTail = NULL;
	pCurThread->pNext=NULL;
	pCurThread->pPrev=ReadyQTail;
	ReadyQTail->pNext=pCurThread;
	ReadyQTail=pCurThread;
}

