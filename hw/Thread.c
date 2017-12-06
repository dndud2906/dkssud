#include "mine.h"
#include "Thread.h"
#include "Init.h"
#include "Scheduler.h"
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>

int 	thread_create(thread_t *thread, thread_attr_t *attr, void *(*start_routine) (void *), void *arg)
{
	WrapperArg wrapperArg;
     wrapperArg.funcPtr = start_routine;
     wrapperArg.funcArg = arg;
	int err;
	if((err=pthread_create(thread,attr,__wrapperFunc,&wrapperArg))!=0)
		fprintf(stderr,"create fail\n");
	InsertAtTail(1,*thread);
	printf("create thead\n");
	sleep(2);
	pthread_kill(*thread, SIGUSR1);

}

int 	thread_join(thread_t thread, void **retval)
{
	
}


int 	thread_suspend(thread_t tid)
{
	Thread* temp=ReadyQHead;
	
	while(temp!=NULL)
	{
		if(temp->tid==tid)
		{
			if((temp!=ReadyQHead)&&(temp!=ReadyQTail))
			{
				InsertAtTail(2,tid);
				Thread* removeThread=temp;
				removeThread->pNext->pPrev=removeThread->pPrev;
				removeThread->pPrev->pNext=removeThread->pNext;
				free(removeThread);
				return 1;
			}
			else if(ReadyQHead==ReadyQTail)
			{
				InsertAtTail(2,tid);
				Thread* removeThread=temp;
				free(removeThread);
				ReadyQHead=NULL;
				ReadyQTail=NULL;
			}
			else if(temp==ReadyQHead)
			{
				InsertAtTail(2,tid);
				Thread* removeThread=temp;
				removeThread->pNext->pPrev=NULL;
				ReadyQHead=removeThread->pNext;
				removeThread->pNext=NULL;
				free(removeThread);
				return 1;
			}
			else if(temp==ReadyQTail)
			{
				InsertAtTail(2,tid);
				Thread* removeThread=temp;
				removeThread->pPrev->pNext=NULL;
				ReadyQTail=removeThread->pPrev;
				removeThread->pPrev=NULL;
				free(removeThread);
				return 1;
			}
		}
		temp=temp->pNext;
	}
}

int	thread_resume(thread_t tid)
{
	Thread* temp=WaitQHead;
	
	while(temp!=NULL)
	{
		if(temp->tid==tid)
		{
			if((temp!=WaitQHead)&&(temp!=WaitQTail))
			{
				InsertAtTail(1,tid);
				Thread* removeThread=temp;
				removeThread->pNext->pPrev=removeThread->pPrev;
				removeThread->pPrev->pNext=removeThread->pNext;
				free(removeThread);
				return 1;
			}
			else if(WaitQHead==WaitQTail)
			{
				InsertAtTail(1,tid);
				Thread* removeThread=temp;
				free(removeThread);
				WaitQHead=NULL;
				WaitQTail=NULL;

				return 1;
			}
			else if(temp==WaitQHead)
			{
				InsertAtTail(1,tid);
				Thread* removeThread=temp;
				removeThread->pNext->pPrev=NULL;
				WaitQHead=removeThread->pNext;
				removeThread->pNext=NULL;
				free(removeThread);
				return 1;
			}
			else if(temp==WaitQTail)
			{
				InsertAtTail(1,tid);
				Thread* removeThread=temp;
				removeThread->pPrev->pNext=NULL;
				WaitQTail=removeThread->pPrev;
				removeThread->pPrev=NULL;
				free(removeThread);
				return 1;
			}
		}
		temp=temp->pNext;
	}
}

thread_t 	thread_self()
{
	return pthread_self();
}


int		thread_exit(void* retval)
{

}

