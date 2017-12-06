#include "TestCase1.h"
#include "Thread.h"

 
void* Tc1ThreadProc(void* param)
{
           thread_t tid = 0;
           int count = 0;
           int* retVal;
 
           tid = thread_self();
 
           count = 5;
           while (count > 0)
           {
                     /* sleep for 1 seconds */
                     sleep(2);
                     printf("Tc1ThreadProc: my thread id (%d)\n",(int)tid,*((int*)param));
                     count--;
           }
           retVal = (int*)param;
           thread_exit(retVal);
           return NULL;
}

/* 
 * - TestCase1 tests a round-robin scheduling of thread  
 * - Testing API scopes: thread_create
 */
void
TestCase1(void)
{
	thread_t tid[TOTAL_THREAD_NUM];
	int i1=1, i2=2, i3=3, i4=4, i5=5;

  thread_create(&tid[0], NULL, (void*)Tc1ThreadProc, (void*)&i1);
  printf("1\n");
  thread_create(&tid[1], NULL, (void*)Tc1ThreadProc, (void*)&i2);
  printf("2\n");
  thread_create(&tid[2], NULL, (void*)Tc1ThreadProc, (void*)&i3);
  printf("3\n");
  thread_create(&tid[3], NULL, (void*)Tc1ThreadProc, (void*)&i4);
  printf("4\n");
  thread_create(&tid[4], NULL, (void*)Tc1ThreadProc, (void*)&i5);
  printf("5\n");

	while(1){}
	
	return ;
}
