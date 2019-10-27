#include <assert.h>
#include <signal.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <stdbool.h>

#include "context.h"
#include "preempt.h"
#include "queue.h"
#include "uthread.h"


/* Size of the stack for a thread (in bytes) */
#define UTHREAD_MAX 32768
#define UTHREAD_STACK_SIZE 32768

enum state {Running, Ready, Blocked, Zombie};
static queue_t running;
static queue_t ready;
static queue_t blocked;
static queue_t zombie;

int num_thread=0;

/* 1. u_tid is the tid for thread
 * 2. parent_tid mean the parent of this thread (will used in thread_join)
 * 3. state will explain to user which state the thread at
 * 4. retval the return value when it is collected by parent
 * 5. u_context, the actual thread
 * 6. u_stack a stack for thread
 */
struct u_thread{
    uthread_t u_tid;
    int parent_tid;
    enum state u_state;
    int retval;
    uthread_ctx_t* u_context;
    void* u_stack;
};

//helper function to find a thread of certain pid
int thread_match_pid(void* data, void* tid){
    //CHANGED HERE : cancel the casting because data is a strcut thread type	
    struct u_thread* curr_ptr = data;
    if(curr_ptr->u_tid == (*(uthread_t*)tid)){
        return 1;
    }
    return 0;
}

void uthread_yield(void)
{
    /* if there is no thread at ready queue and running queue then function ends
     */
    if (queue_length(ready) == 0 || queue_length(running) == 0)
        return;
    struct u_thread* will_run = NULL;
    struct u_thread* will_yield = NULL;

    int deqval1 = queue_dequeue(running,(void**)&will_yield);
    int deqval2 = queue_dequeue(ready,(void**)&will_run);

    if (deqval1 != -1 && deqval2 != -1) {
        will_run->u_state = Running;
        will_yield->u_state = Ready;
        queue_enqueue(running, will_run);
        queue_enqueue(ready, will_yield);

        preempt_disable();
        uthread_ctx_switch(will_yield->u_context, will_run->u_context);
        preempt_enable();
    }else{
        return;
    }
 }

uthread_t uthread_self(void)
{
    if (queue_length(running) == 0) {
        return -1;
    }
    if (queue_length(running) > 1) {
        printf("more than 1 running thread. uthread_self doesn't work.\n");
        return -1;
    }
    struct u_thread* curr = NULL;
    queue_dequeue(running, (void**)&curr);
    queue_enqueue(running, curr);
    return curr->u_tid;
}

/* intialize the main thread
 * this function will call when intialize = 0 */
void initialize()
{
    /* create 4 queue list */
  	running = queue_create();
  	ready = queue_create();
  	blocked = queue_create();
  	zombie = queue_create();

    /* intialize main_thread
     * 1. set main thread tid = 0
     * 2. set it parent tid = -1, it will never assign to other value
     * 3. set state to running
     * 4. set retval = 0
     * 5. create a stack for the thread
     * 6. create actual thread
     * 7. assign thread stack */
  	struct u_thread* main_thread = (struct u_thread*)malloc(sizeof(struct u_thread));
  	main_thread->u_tid = num_thread;//which is 0 right now
    main_thread->parent_tid =-1;
  	main_thread->u_state = Running;
  	main_thread->retval=0;

  	main_thread->u_stack = uthread_ctx_alloc_stack();
    main_thread->u_context = (uthread_ctx_t*)malloc(sizeof(uthread_ctx_t));
  	main_thread->u_context->uc_stack.ss_sp = main_thread->u_stack;
  	main_thread->u_context->uc_stack.ss_size = UTHREAD_STACK_SIZE;
  	queue_enqueue(running,main_thread);

    preempt_start();
  	return;
}

int uthread_create(uthread_func_t func, void *arg)
{
  	if(num_thread == 0)
          initialize();
  	if(num_thread++>UTHREAD_MAX){
          fprintf(stderr,"ERROR: TID Overflow!\n");
  	      return -1;
  	}
    /* Create a new thread
     * assign value to new thread, same as initialize() */
    //printf("before malloc, increate\n");
    struct u_thread* new_thread = (struct u_thread*)malloc(sizeof(struct u_thread));
    //printf("after malloc created\n");
    new_thread->u_tid = num_thread;
    new_thread->parent_tid = -1;
    new_thread->u_state = Ready;
    new_thread->retval = 0;
    new_thread->u_stack = uthread_ctx_alloc_stack();
    new_thread->u_context = (void*)malloc(sizeof(uthread_ctx_t));

    int curr_retval = uthread_ctx_init(new_thread->u_context,new_thread->u_stack,func,arg);
    if(curr_retval==-1){
        return curr_retval;
    }else{
        queue_enqueue(ready, new_thread);
        return num_thread;
    }
}

void uthread_exit(int retval)
{
    struct u_thread* will_exit = NULL;
    struct u_thread* will_run = NULL;
    int deqval1 = queue_dequeue(running,(void**)&will_exit);
    int deqval2 = queue_dequeue(ready,(void**)&will_run);

    if(deqval1 != -1 && deqval2 != -1) {
        if(will_exit->parent_tid > -1) {
            struct u_thread* parent_thread = NULL;
            queue_iterate(blocked,thread_match_pid, (void*)&will_exit->parent_tid, (void**)&parent_thread);

            /* if the queue_iterate indicate the thread have no parent then continue
             * if not it will call it parent for block queue and put it to Ready
             */
            if(parent_thread != NULL) {
                parent_thread->u_state = Ready;
                queue_delete(blocked,parent_thread);
                queue_enqueue(ready,parent_thread);
            }
        }
        will_exit->retval = retval;
        will_exit->u_state = Zombie;
        will_run->u_state = Running;
        queue_enqueue(zombie,will_exit);
        queue_enqueue(running,will_run);

        preempt_disable();
        uthread_ctx_switch(will_exit->u_context, will_run->u_context);
        preempt_enable();
    }else { return; }
}

int uthread_join(uthread_t tid, int *retval)
{
    uthread_t self_tid = uthread_self();
    struct u_thread* child_thread = NULL;
    struct u_thread* parent_thread = NULL;

    /* To prevent main joining itself and pervent itself joining itself
     */
    if(tid == 0 || tid == self_tid) {
      return -1;
    }

    /* To check if child is in zombie state
     * If the child is already in zombie state, then it will collect and free
     * child then it will return 0 as it succuss.
     */
    queue_iterate(zombie, thread_match_pid, (void*)&tid, (void**)&child_thread);
    if(child_thread != NULL){
      /* if already has a parent */
      if(child_thread->parent_tid > -1){
        return -1;
      }
      *retval = child_thread->retval;
      queue_delete(zombie, child_thread);
      return 0;
    }
    //printf("in uthread_join tid is%d child is not zombie\n", tid);

    /* To check if child is in ready or running state
     * If the child is in those state, its parent will put into block state
     * wait until the child die.
     * Condition 1: if child is NULL, then return -1 because it not in ready and Running
     * Condition 2: if child has parent then it will also return -1
     */
    queue_iterate(ready, thread_match_pid, (void*)&tid, (void**)&child_thread);
    queue_iterate(blocked, thread_match_pid, (void*)&tid, (void**)&child_thread);
    //printf("in uthread_join child tid is %d after two iteration\n", tid);
    if(child_thread == NULL)
        return -1;
      /* if the child already has a parent, return false */
    if(child_thread->parent_tid > -1)
        return -1;
    //printf("in uthread_join child tid is%d in blocked or ready\n", tid);
    child_thread->parent_tid = self_tid;

    struct u_thread* willrun = NULL;
    int retdeq1=queue_dequeue(running,(void**)&parent_thread);
    int retdeq2=queue_dequeue(ready,(void**)&willrun);
    if(retdeq1 != -1 && retdeq2 != -1) {
        parent_thread->u_state = Blocked;
        willrun->u_state = Running;
        queue_enqueue(blocked,parent_thread);
        queue_enqueue(running,willrun);
        preempt_disable();
        uthread_ctx_switch(parent_thread->u_context, willrun->u_context);
        preempt_enable();
        /* detect if @retval args is a NULL, then assign value
         * back from blocked, child dead */
        if(retval != NULL)
            *retval = child_thread->retval;
        queue_delete(zombie, child_thread);
        return 0;
    }
    return -1;
}
