/*
 * test_queue
 *
 * Tests the queue struct
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "queue.h"

static int inc_item(void *data, void *arg)
{
    int *a = (int*)data;
    int inc = (int)(long)arg;

    *a += inc;

    return 0;
}

/* Callback function that finds a certain item according to its value */
static int find_item(void *data, void *arg)
{
    int *a = (int*)data;
    int match = (int)(long)arg;

    if (*a == match)
        return 1;

    return 0;
}

void test_iterator(void)
{
    queue_t q;
    int data[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    int i;
    int *ptr;

    /* Initialize the queue and enqueue items */
    q = queue_create();
    for (i = 0; i < sizeof(data) / sizeof(data[0]); i++)
        queue_enqueue(q, &data[i]);

    /* Add value '1' to every item of the queue */
    queue_iterate(q, inc_item, (void*)1, NULL);
    assert(data[0] == 2);

    /* Find and get the item which is equal to value '5' */
    ptr = NULL;
    queue_iterate(q, find_item, (void*)5, (void**)&ptr);
    assert(ptr != NULL);
    assert(*ptr == 5);
    assert(ptr == &data[3]);
    printf("completed test iteration\n");
}

void test_create(void)
{
    queue_t q;

    q = queue_create();
    assert(q != NULL);
    printf("completed test create\n");
}

void test_queue_enqueue(void)
{
    queue_t q, *ptr;
    int data[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    q = queue_create();
    for (int i = 0; i < sizeof(data) / sizeof(data[0]); i++)
        queue_enqueue(q, &data[i]);

    for (int i = 0; i < sizeof(data) / sizeof(data[0]); i++){
        queue_dequeue(q, (void**)&ptr);
        assert(ptr == &data[i]);
    }

    printf("completed test queue queue_enqueue & dequeue\n");
}

void test_queue_delete(void)
{
    queue_t q;
    int data[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    q = queue_create();
    for (int i = 0; i < sizeof(data) / sizeof(data[0]); i++)
        queue_enqueue(q, &data[i]);

    for (int i = 0; i < sizeof(data) / sizeof(data[0]); i++)
        assert(queue_delete(q, (void*)&data[i]) == 0);
    assert(q == NULL);

    printf("completed test queue delete\n");
}

int main(void)
{
    test_create();
    test_queue_enqueue();
    test_queue_delete();
	  return 0;
}
