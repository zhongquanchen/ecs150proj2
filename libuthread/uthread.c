#include <assert.h>
#include <signal.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#include "context.h"
#include "preempt.h"
#include "queue.h"
#include "uthread.h"

#define UTHREAD_STACK_SIZE 32768
#define USHRT_MAX 32767

enum state {Running, Ready, Blocked, Zombie};
static queue_t running;
static queue_t ready;
static queue_t blocked;
static queue_t zombie;
int initialized =0;
int num_thread=0;

struct thread{
  uthread_t u_tid;
  enum state u_state;
  int retval;
  uthread_ctx_t* u_context;
  void* u_stack;
};

void uthread_yield(void)
{
	/* TODO Phase 2 */
}

uthread_t uthread_self(void)
{
	/* TODO Phase 2 */
}

void initialize()
{
	initialized=1;

	running = queue_create();
	ready = queue_create();
	blocked = queue_create();
	zombie = queue_create();

	struct thread* main = malloc(sizeof(struct thread));

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

	if(num_thread++>USHRT_MAX){
		fprintf(stderr,"ERROR: TID Overflow!\n");
		return -1;
	}

	struct thread* new_thread = malloc(sizeof(struct thread));

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
