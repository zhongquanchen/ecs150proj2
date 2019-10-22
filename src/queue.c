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
    return my_queue;
}

int queue_destroy(queue_t queue)
{
	/* TODO Phase 1 */
}

int queue_enqueue(queue_t queue, void *data)
{
  if(!queue->size){
    struct ele_q* new_element = malloc(sizeof(struct ele_q));
    new_element->element_ptr = data;
    queue->oldest = new_element;
    queue->newest = new_element;
    queue->size++;
    return 0;
  }
  printf("size not zero\n");
  struct ele_q* new_element = malloc(sizeof(struct ele_q));
  new_element->element_ptr = data;
  queue->newest->next_ptr = new_element;
  queue->newest = new_element;
  queue->size++;
  return 0;
}

int queue_dequeue(queue_t queue, void **data)
{
	if(queue->size==1){
    *data = queue->oldest->element_ptr;
    free(queue->oldest);
    free(queue->newest);
    queue->oldest = queue->newest = malloc(sizeof(struct ele_q));
    queue->size--;
    return 0;
  }
  *data = queue->oldest->element_ptr;
  struct ele_q* old_element = malloc(sizeof(struct ele_q));
  old_element = queue->oldest;
  queue->oldest = old_element->next_ptr;
  free(old_element);
  queue->size--;
  return 0;
}

int queue_delete(queue_t queue, void *data)
{
	/* TODO Phase 1 */
}

int queue_iterate(queue_t queue, queue_func_t func, void *arg, void **data)
{
	/* TODO Phase 1 */
}

int queue_length(queue_t queue)
{
	/* TODO Phase 1 */
}

int main(){
  queue_t myQptr = queue_create();
  void* addr1 = 1;
  void* addr2 = 4;
  int retval = queue_enqueue(myQptr,addr1);
  printf("the newest element contains addr%p\n",myQptr->newest->element_ptr);
  retval = queue_enqueue(myQptr,addr2);
  printf("the newest element contains addr%p\n",myQptr->newest->element_ptr);
  printf("retval is, %d\n", retval);
  printf("size is, %d\n", myQptr->size);
  void* newaddr1;
  void* newaddr2;
  queue_dequeue(myQptr,&newaddr1);
  printf("popped oldest element contains addr%p\n", newaddr1);
  queue_dequeue(myQptr,&newaddr2);
  printf("popped oldest element contains addr%p\n", newaddr2);
}
