/*
 * Thread creation and yielding test
 *
 * Tests the creation of multiples threads and the fact that a parent thread
 * should get returned to before its child is executed. The way the printing,
 * thread creation and yielding is done, the program should output:
 *
 * thread1
 * thread2
 * thread3
 */

#include <stdio.h>
#include <stdlib.h>

#include "uthread.h"

int thread3(void* arg)
{
	printf("first time in thread3\n");
	uthread_yield();
	return 0;
}

int thread2(void* arg)
{
	uthread_t tid = uthread_create(thread3, NULL);
	int* retval_ptr = malloc(sizeof(int));
	uthread_join(tid, retval_ptr);
	printf("retval_ptr %d\n", retval_ptr);
	uthread_yield();
	return 0;
}

int thread1(void* arg)
{
	uthread_create(thread2, NULL);
	uthread_yield();
	printf("thread1 is back, will exit soon\n");
	return 0;
}

int main(void)
{
	int* retval_ptr = malloc(sizeof(int));
	printf("enter main\n");
	uthread_join(uthread_create(thread1, NULL), retval_ptr);
	return 0;
}
