#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>

#include "queue.h"

struct ele_q{
    void* element_ptr;
    struct ele_q* next_ptr;
};

struct queue {
    struct ele_q* oldest;
    struct ele_q* newest;
    int size;
};

queue_t queue_create(void)
{
    queue_t my_queue;
    my_queue = malloc(sizeof(struct queue));
    my_queue->oldest = my_queue->newest = malloc(sizeof(struct ele_q));
    my_queue->size = 0;
    return my_queue;
}

int queue_destroy(queue_t queue)
{
    while(queue_length(queue) != 0){
        void* nulladdr;
        queue_dequeue(queue, &nulladdr);
    }
    free(queue);
    return 0;
}

int queue_enqueue(queue_t queue, void *data)
{
    /* malloc a new element location and assert new_ele for it property */
    /* assert will automatic detect & abort function if malloc not success */
    struct ele_q* new_element = malloc(sizeof(struct ele_q));
    assert(new_element);
    
    if(!queue->size){
        new_element->element_ptr = data;
        queue->oldest = queue->newest = new_element;
        queue->size++;
    } else {
        new_element->element_ptr = data;
        queue->newest->next_ptr = new_element;
        queue->newest = new_element;
        queue->size++;
    }
    return 0;
}

int queue_dequeue(queue_t queue, void **data)
{
    if(queue_length(queue) == 0){
        /* Return: -1 if @queue or @data are NULL, or if the queue is empty. */
        return -1;
    }
	if(queue->size==1){
        *data = queue->oldest->element_ptr;
        free(queue->oldest);
        queue->oldest = queue->newest = malloc(sizeof(struct ele_q));
    } else {
        *data = queue->oldest->element_ptr;
        struct ele_q* old_element = malloc(sizeof(struct ele_q));
        old_element = queue->oldest;
        queue->oldest = old_element->next_ptr;
        free(old_element);
    }
    queue->size--;
    return 0;
}

int queue_delete(queue_t queue, void *data)
{
    struct ele_q *temp = queue->oldest;
    for(int i=0; i<queue->size; i++) {
        if (temp->element_ptr == data) {
            //MARK:- do the following
            printf("queue detected %d; adr is %p\n", temp->element_ptr, temp->element_ptr);
            
        }
        temp = temp->next_ptr;
    }
    return -1;
}

int queue_iterate(queue_t queue, queue_func_t func, void *arg, void **data)
{
    return 0;
}

int queue_length(queue_t queue)
{
    if(queue->oldest == NULL){
        return 0;
    }
    return queue->size;
}

int main(){
    queue_t myQptr = queue_create();
    void* addr1 = 1;
    void* addr2 = 10;
    int retval = queue_enqueue(myQptr,addr1);
    printf("retval %d\n", retval);
    printf("size is, %d\n", queue_length(myQptr));
    retval = queue_enqueue(myQptr,addr2);
    
    printf("the oldest element contains addr%d\n",myQptr->oldest->element_ptr);
    printf("the newest element contains addr%d\n",myQptr->newest->element_ptr);
    
    void* newaddr1;
    void* newaddr2;
    
//    queue_dequeue(myQptr,&newaddr1);
//    printf("popped oldest element contains addr %d\n", newaddr1);
//    queue_dequeue(myQptr,&newaddr2);
//    printf("popped oldest element contains addr%p\n", newaddr2);
//
    printf("retval %d\n", retval);
    printf("size %d\n", queue_length(myQptr));
    
    queue_delete(myQptr, 10);
    
    queue_destroy(myQptr);
}
