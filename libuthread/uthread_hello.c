/*
 * Simple hello world test
 *
 * Tests the creation of a single thread and its successful return.
 */

#include <stdio.h>
#include <stdlib.h>

#include "uthread.h"

int hello2(void* arg)
{
    printf("at hello 2!\n");
    return 0;
}

int hello(void* arg)
{
	printf("Hello world!\n");
    uthread_t tid = uthread_create(hello2, NULL);
    printf("after create before yield\n");
    uthread_yield();
    printf("after created thread\n");
	return 0;
}

int main(void)
{
	uthread_t tid;

	tid = uthread_create(hello, NULL);
	uthread_join(tid, NULL);

	return 0;
}
