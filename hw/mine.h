#ifndef __MINE_H__
#define __MINE_H__
#include "Thread.h"


Thread* getNewThread(thread_t id);
void DeleteAtHead(int value);
void InsertAtTail(int value,thread_t id);
void PrintQ(int value);
Thread* __getThread(thread_t tid);
Thread* searchThread(int value,thread_t tid);
void* __wrapperFunc(void* arg);
void __thread_wait_handler(int signo);
void __thread_wakeup(Thread* pTh);

#endif 