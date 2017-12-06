#include "mine.h"
#include "Init.h"
#include "Scheduler.h"
#include "Thread.h"
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>

pthread_cond_t cinit=PTHREAD_COND_INITIALIZER;
pthread_mutex_t minit=PTHREAD_MUTEX_INITIALIZER;



Thread* getNewThread(thread_t id)
{
	Thread *newThread=(Thread*)malloc(sizeof(Thread));
	newThread->pPrev=NULL;
	newThread->pNext=NULL;
	newThread->status=THREAD_STATUS_READY;
	newThread->bRunnable=0;
	newThread->tid=id;
	newThread->readyCond=cinit;
	newThread->readyMutex=minit;

	return newThread;
}

void InsertAtTail(int value,thread_t id)
{
	if(value==1)
	{

		Thread* temp=ReadyQHead;
		Thread* newThread=getNewThread(id);
		
		if(ReadyQHead==NULL)
		{
			ReadyQHead=newThread;
			ReadyQTail=newThread;
			return;
		}
		else
		{
			while(temp->pNext!=NULL)
			{
				temp=temp->pNext;
			}
			temp->pNext=newThread;
			newThread->pPrev=temp;
			ReadyQTail=newThread;
		}
	}
	else if(value==2)
	{
		Thread* temp=WaitQHead;
		Thread* newThread=getNewThread(id);
		newThread->status=THREAD_STATUS_BLOCKED;
	
		if(WaitQHead==NULL)
		{
			WaitQHead=newThread;
			WaitQTail=newThread;
			return;
		}
		else
		{
			while(temp->pNext!=NULL)
			{
				temp=temp->pNext;
			}
			temp->pNext=newThread;
			newThread->pPrev=temp;
			WaitQTail=newThread;
		}
	}
}
void PrintQ(int value)
{
	if(value==1)
	{
		Thread* temp=ReadyQHead;
		int i=0;
		printf("Ready Q---------------------------------------------------\n");
		printf("Head: %p\n",temp);
		while(temp!=NULL)
		{
			printf("prev[%d](%p) : %p, next[%d] : %p\n",i,temp,temp->pPrev,i,temp->pNext);
			printf("this tid[%d]->%ud\n",i,(unsigned int)temp->tid);
			printf("status->%d\n",temp->status);
			printf("bRunnable->%d\n",temp->bRunnable);
			temp=temp->pNext;
			i++;
		}
		printf("\n");
	}
	else if(value==2)
	{
		Thread* temp=WaitQHead;
		int i=0;
		printf("Wait Q---------------------------------------------------\n");
		printf("Head: %p\n",temp);
		while(temp!=NULL)
		{
			printf("prev[%d](%p) : %p, next[%d] : %p\n",i,temp,temp->pPrev,i,temp->pNext);
			printf("this tid[%d]->%ud\n",i,(unsigned int)temp->tid);
			printf("status->%d\n",temp->status);
			temp=temp->pNext;
			i++;
		}
		printf("\n");
	}
}

void DeleteAtHead(int value)
{
	if(value==1)
	{
		if(ReadyQHead==NULL)
		{
			return;
		}
		Thread* temp=ReadyQHead;
		ReadyQHead=temp->pNext;
		free(temp);
		if(ReadyQHead==NULL)
		{
			return;
		}
		ReadyQHead->pPrev=NULL;
		ReadyQTail=NULL;
	}
	else if(value==2)
	{
		if(WaitQHead==NULL)
		return;

		Thread* temp=WaitQHead;
		WaitQHead=temp->pNext;
		free(temp);
		if(WaitQHead==NULL)
		{
			return;
		}
		WaitQHead->pPrev=NULL;
		WaitQTail=NULL;
	}
}

void* __wrapperFunc(void* arg)
{
  void* ret;
  WrapperArg* pArg = (WrapperArg*)arg;
  void*  (*funcPtr)(void*)=pArg->funcPtr;
  void* funcArg=pArg->funcArg;
  sigset_t set;
  int retSig;  
  //sleep(1);
  printf("wrap\n");
  // child sleeps until TCB is initialized
  sigemptyset(&set);
  sigaddset(&set, SIGUSR1);
  signal(SIGUSR1, __thread_wait_handler);
  printf("wait\n");
  sigwait(&set, &retSig);
  // child is ready to run 
  printf("child run\n");
  __thread_wait_handler(0);
  ret = (*funcPtr)(funcArg);
  return ret;
}

void __thread_wait_handler(int signo)
{
   Thread* pTh;
   printf("handler in\n");
   pTh = __getThread(pthread_self());
   printf("self\n");
   pthread_mutex_lock(&(pTh->readyMutex));
   printf("lock\n");
   while (pTh->bRunnable == FALSE)
      pthread_cond_wait(&(pTh->readyCond), &(pTh->readyMutex));
   pthread_mutex_unlock(&(pTh->readyMutex));
   printf("handler out\n");
}

Thread* __getThread(thread_t tid)
{
	searchThread(1,tid);
}
Thread* searchThread(int value,thread_t tid)
{
	if(value==1)
	{
		Thread* temp=ReadyQHead;
		while(temp!=NULL)
		{
			if(temp->tid==tid)
			{
				return temp;
			}
			temp=temp->pNext;
		}
		return NULL; // 없으면 NULL 전달
	}
	else if(value==2)
	{
		Thread* temp=WaitQHead;
		while(temp!=NULL)
		{
			if(temp->tid==tid)
			{
				return temp;
			}
			temp=temp->pNext;
		}
		return NULL; // 없으면 NULL 전달
	}
}

void __thread_wakeup(Thread* pTh)
{
	pthread_mutex_lock(&(pTh->readyMutex));
	pTh->bRunnable = TRUE;
	pthread_cond_signal(&(pTh->readyCond));
	pthread_mutex_unlock(&(pTh->readyMutex));
	printf("wake\n");
}