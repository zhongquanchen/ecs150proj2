/*
 * Simple hello world test
 *
 * Tests the creation of a single thread and its successful return.
 */

#include <stdio.h>
#include <stdlib.h>

#include "uthread.h"

int hello3(void* arg)
{
    printf("hello from hello 3\n");
    return 0;
}

int hello2(void* arg)
{
    printf("hello from hello 2\n");
    uthread_yield();
    printf("hello 2 after yield\n");
    return 0;
}

int hello1(void* arg)
{
    print("hello from hello 1\n");
    uthread_yield();
    printf("hello 1 after yield\n")
	  return 0;
}

int main(void)
{
	uthread_t tid1, tid2, tid3;
  int* retval_ptr = malloc(sizeof(int));
	tid1 = uthread_create(hello1, NULL);
  tid2 = uthread_create(hello2, NULL);
  tid3 = uthread_create(hello3, NULL);

	uthread_join(tid3, retval_ptr);
  printf("exit after 3\n");
  printf("zombie length %d\n", queue_length(zombie));

	return 0;
}
