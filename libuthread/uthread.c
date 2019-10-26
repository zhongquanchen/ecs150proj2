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

struct TCB{
  uthread_t u_tid;
  enum state u_state;
  int retval;
  uthread_ctx_t* u_context;
  void* u_stack;
};

typedef struct TCB u_thread;

static u_thread* main_thread;
/* tells create_thread() to create main thread first */
static int init_tid = 0;

void uthread_yield(void)
{
	/* TODO Phase 2 */
}

uthread_t uthread_self(void)
{
	/* TODO Phase 2 */
}

/* intialize the main thread */
void initialize()
{
    /* FIXME- need to add the running state for main thread later */
	initialized=1;
	running = queue_create();
	ready = queue_create();
	blocked = queue_create();
	zombie = queue_create();
	u_thread* main = malloc(sizeof(u_thread));
	main->u_tid = num_thread;//which is 0 right now
	main->u_state = Running;
	main->retval=0;
	main->u_context = malloc(sizeof(uthread_ctx_t));
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

	u_thread* new_thread = malloc(sizeof(u_thread));
	new_thread->u_tid = num_thread;
	new_thread->u_state = Ready;
	new_thread->retval = 0;
	new_thread->u_context = malloc(sizeof(uthread_ctx_t));
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
	/* TODO Phase 2 */
}

int uthread_join(uthread_t tid, int *retval)
{
	/* TODO Phase 2 */
	/* TODO Phase 3 */
}
