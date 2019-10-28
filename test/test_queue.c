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
    printf("queue_iterate succeeded\n");
}

void test_destroy(queue_t t){
    int *a;
    while(queue_length(t) > 0 ){
        queue_dequeue(t,(void**)&a);
    }
    int b = queue_destroy(t);
    if(b== 0){
        printf("queue_destroy succeeded\n");
    }
    else{
        printf("queue_destroy failed\n");
    }

}

void test_dequeue(queue_t q){
    int *a;
    queue_dequeue(q,(void**)&a);
    if(*a == 1){
        printf("queue_dequeue succeeded\n");
    }
}

void test_delete(queue_t q, void *data){
    int beforeDel = queue_length(q);
    int afterDel;
    int del = queue_delete(q,data);
    if(del == 0){
        afterDel = queue_length(q);
        if(beforeDel>afterDel){
            printf("queue_delete succeeded\n");
        }
        else{
            printf("queue_delete failed\n");
        }
    }

}


void test(){
    queue_t q;
    int test_data[] ={1,2,3,4,5,6,7,8,9,10};
    q = queue_create();
    for (int i = 0; i < sizeof(test_data) / sizeof(test_data[0]); i++){
        queue_enqueue(q, &test_data[i]);
    }
    if(queue_length(q) == 10){
        printf("queue_length succeeded\n");
    }
    else{
        printf("queue_length failed\n");
    }
    printf("queue_enqueue succeeded\n");
    test_dequeue(q);
    test_delete(q,&test_data[2]);
    test_destroy(q);

}


int main(){
    test();
    test_iterator();

        return 0;
}
