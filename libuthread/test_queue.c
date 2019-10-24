/*
 * test_queue
 *
 * Tests the queue struct
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "queue.h"

void test_create(void)
{
  queue_t q;

  q = queue_create();
  assert(q != NULL);
  printf("completed test create\n");
}

void test_queue_destory(void)
{
    queue_t q;
    int data = 3, *ptr;

    q = queue_create();
    queue_enqueue(q, &data);
    queue_destroy(q);
    assert(q );
    printf("completed test queue destory\n");
}

void test_queue_enqueue(void)
{
    queue_t q;
    int data = 3, *ptr;
    int data2 = 10;
    int data3 = 11;

    q = queue_create();
    queue_enqueue(q, &data);
    queue_enqueue(q, &data2);
    queue_enqueue(q, &data3);

    queue_dequeue(q, (void**)&ptr);
    assert(ptr == &data);
    queue_dequeue(q, (void**)&ptr);
    assert(ptr == &data2);
    queue_dequeue(q, (void**)&ptr);
    assert(ptr == &data3);
    printf("completed test queue queue_enqueue & dequeue\n");
}

int main(void)
{
  test_queue_destory();
	test_create();
  test_queue_enqueue();

	return 0;
}
