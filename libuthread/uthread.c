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
int initialized =0;
int num_thread=0;

struct u_thread{
  uthread_t u_tid;
  uthread_t parent_tid;
  enum state u_state;
  int retval;
  uthread_ctx_t* u_context;
  void* u_stack;
};


static int init_tid = 0;

//helper function to find a thread of certain pid
int thread_match_pid(void*data, void*tid){
  struct u_thread* curr_ptr = (struct thread*)data;
  if(curr_ptr->u_tid == (*(uthread_t*)tid)){
    return 1;
  }
  return 0;
}

void uthread_yield(void)
{
    if(queue_length(ready) == 0|| queue_length(running) == 0)
        return;
    struct u_thread* willrun = NULL;
    struct u_thread* willyield = NULL;

    int deqval1 = queue_dequeue(running,(void**)&willyield);
    int deqval2 = queue_dequeue(ready,(void**)&willrun);

    if(deqval1!=-1&&deqval2!=-1){
        willrun->u_state = Running;
        willyield->u_state = Ready;
        queue_enqueue(running,willrun);
        queue_enqueue(ready,willyield);
        uthread_ctx_switch(willyield->u_context, willrun->u_context);
    }else{
        return;
    }
 }

uthread_t uthread_self(void)
{
  if(queue_length(running)==0){
    return -1;
  }
  if(queue_length(running)>1){
    printf("more than 1 running thread. uthread_self doesn't work.\n");
    return -1;
  }
  struct u_thread* curr = NULL;
  queue_dequeue(running,(void**)&curr);
  queue_enqueue(running,curr);
  return curr->u_tid;
}

/* intialize the main thread
 * this function will call when intialize = 0 */
void initialize()
{
	initialized=1;
	running = queue_create();
	ready = queue_create();
	blocked = queue_create();
	zombie = queue_create();
	struct u_thread* main = (struct u_thread*)malloc(sizeof(struct u_thread));
	main->u_tid = num_thread;//which is 0 right now
  main->parent_tid =-1;
	main->u_state = Running;
	main->retval=0;
	main->u_context = (uthread_ctx_t*)malloc(sizeof(uthread_ctx_t));
	main->u_stack = uthread_ctx_alloc_stack();
	main->u_context->uc_stack.ss_sp = main->u_stack;
	main->u_context->uc_stack.ss_size = UTHREAD_STACK_SIZE;
	queue_enqueue(running,main);
	return;
}

int uthread_create(uthread_func_t func, void *arg)
{
	if(!initialized){
		initialize();
	}
	if(num_thread++>UTHREAD_MAX){
		fprintf(stderr,"ERROR: TID Overflow!\n");
		return -1;
	}
    //printf("before malloc, increate\n");
	struct u_thread* new_thread = (struct u_thread*)malloc(sizeof(struct u_thread));
    //printf("after malloc created\n");
	new_thread->u_tid = num_thread;
  new_thread->parent_tid = -1;
	new_thread->u_state = Ready;
	new_thread->retval = 0;
	new_thread->u_context = (void*)malloc(sizeof(uthread_ctx_t));
	new_thread->u_stack = uthread_ctx_alloc_stack();
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
  struct u_thread* willexit=NULL;
  struct u_thread* willrun=NULL;
  int deqval1 = queue_dequeue(running,(void**)&willexit);
  int deqval2 = queue_dequeue(ready,(void**)&willrun);

  if(deqval1!=-1&deqval2!=-1){
    if(willexit->parent_tid>-1){
      struct* u_thread parent_thread = NULL;
      queue_iterate(blocked,thread_match_pid,(void*)&willexit->parent_tid,(void**)&parent_thread);
      if(parent_thread==NULL){
        printf("can't find parent.\n");
      }else{
        parent_thread->u_state = Ready;
        queue_delete(blocked,parent);
        queue_enqueue(ready,parent);
      }
    }
    willexit->retval = retval;
    willexit->u_state = Zombie;
    willrun->u_state = Running;
    queue_enqueue(zombie,willexit);
    queue_enqueue(running,willrun);
    uthread_ctx_switch(willexit->u_context, willrun->u_context);
  }else{
    return;
  }
}

int uthread_join(uthread_t tid, int *retval)
{
  //placeholder
	//while(1){
    //printf("length of running is%d\n",queue_length(running));
    //printf("length of ready is%d\n",queue_length(ready));
    //printf("main yielding\n");
    //if(queue_length(ready)==0){
      //return(-1);
    //}
    //uthread_yield();
  //}

    self_tid = uthread_self();
    struct u_thread* child_thread = NULL;
    struct u_thread* parent_thread = NULL;
    //if joining with main or with self
    if(tid==0||tid==self_tid){
      return -1;
    }

    //find out if child is already dead
    queue_iterate(zombie, thread_match_pid, (void*)&tid, (void**)&child_thread);
    if(child_thread!=NULL){
      //if already has a parent
      if(child_thread->parent_tid!=-1){
        return -1;
      }
      //save retval and delete child from the zombie queue;
      *retval = child_thread->retval;
      queue_delete(zombie, child_thread);
      return 0;
    }

    //find out if child is in ready or Blocked
    queue_iterate(ready, thread_match_pid, (void*)&tid, (void**)&child_thread);
    queue_iterate(blocked, thread_match_pid, (void*)&tid, (void**)&child_thread);
    if(child_thread==NULL){
      //if can't find child anywhere
      return -1;
    }else{
      //if already has a parent
      if(child_thread->parent_tid!=-1){
        return -1;
      }
      child_thread->parent_tid = self_tid;
      //put parent in blocked, bring up first in Ready
      struct u_thread* willrun = NULL;
      int retdeq1=queue_dequeue(running,(void**)&parent_thread);
      int retdeq2=queue_dequeue(ready,(void**)&willrun);
      if(retdeq1!=-1&&retdeq2!=-1){
        parent_thread->u_state = Blocked;
        willrun->u_state = Running;
        queue_enqueue(blocked,parent_thread);
        queue_enqueue(running,willrun);
        uthread_ctx_switch(parent_thread->u_context, willrun->u_context);

        //back from blocked, child dead
        *retval = child_thread->retval;
        queue_delete(zombie, child_thread);
        return 0;
      }
    }
    return -1;
}
