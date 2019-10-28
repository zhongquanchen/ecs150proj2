#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
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
    printf("test_iterator test succuss\n");
}

void test_destroy(void){
    queue_t q;
    int data[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    int i;
    int *a;
    /* Initialize the queue and enqueue items */
    q = queue_create();
    for (i = 0; i < sizeof(data) / sizeof(data[0]); i++)
        queue_enqueue(q, &data[i]);

    while(queue_length(q) > 0 ){
        queue_dequeue(q,(void**)&a);
    }
    int b = queue_destroy(q);
    assert(b == 0);
    printf("queue_destroy succeeded\n");
}

void test_dequeue(void){
    int *a;
    int data = 10;
    queue_t q = queue_create();
    queue_enqueue(q, &data);
    queue_dequeue(q,(void**)&a);

    assert(*a == 10);
    printf("queue_dequeue succeeded\n");
}

/* test the queue deletion */
void test_delete(void){
    queue_t q;
    int data[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    int i;
    /* Initialize the queue and enqueue items */
    q = queue_create();
    for (i = 0; i < sizeof(data) / sizeof(data[0]); i++)
        queue_enqueue(q, &data[i]);

    int origin_len = queue_length(q), after_len;
    int del = queue_delete(q, &data[2]);

    assert(del == 0);
    after_len = queue_length(q);
    assert(origin_len > after_len);
    printf("queue_delete succeeded\n");
}

/* include all the test case */
void test(){
    test_delete();
    test_dequeue();
    test_destroy();
    test_iterator();
}

int main(){
    test();
    return 0;
}
