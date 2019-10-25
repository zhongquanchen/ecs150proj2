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
#define UTHREAD_STACK_SIZE 32768
#define UTHREAD_MAX 32768

enum state {running, ready, blocked, dead};

struct TCB{
  uthread_t u_tid;
  enum state u_state;
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
int initialize()
{
    /* FIXME- need to add the running state for main thread later */
    main_thread = malloc(sizeof(u_thread));
    assert(main_thread);
    main_thread->u_tid = 0;
    main_thread->u_stack = uthread_ctx_alloc_stack();
    main_thread->u_context->uc_stack.ss_sp = main_thread->u_stack;
    main_thread->u_context->uc_stack.ss_size = UTHREAD_STACK_SIZE;
    return 1;
}

int uthread_create(uthread_func_t func, void *arg)
{
    /* check it init_tid == 0 to see have the main thread initialized */
    if(init_tid == 0){
        initialize();
    }
    init_tid++;
    
    if(init_tid > UTHREAD_MAX)
        return -1;
    
    /* create the thread */
    u_thread* temp_thread = malloc(sizeof(u_thread));
    temp_thread->u_tid = init_tid;
    temp_thread->u_stack = uthread_ctx_alloc_stack();
    int succeed = uthread_ctx_init(temp_thread->u_context, temp_thread->u_stack, func, arg);
    assert(succeed == 0);
    return 0;
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
