# REPORT #

## Phase 1 ##
- Queue implement
	- Detele 
	We implement the queue just like normal queue struct. For the 
	[queue_delete()](https://github.com/zhongquanchen/ecs150proj2/blob/a91cac83b8121994c44e50d56bd17bd812bd6ccc/libuthread/queue.c#L77), we compare the address of two value, if it points to
	the same address, then it will detele that pointer in the queue.

	- Iterate
	For [queue_iterate()](https://github.com/zhongquanchen/ecs150proj2/blob/a91cac83b8121994c44e50d56bd17bd812bd6ccc/libuthread/queue.c#L111), we just iterate all the value in the queue and 
	return it to the pointer of ` (void**) retval ` 

## Phase 2&3 ##
- UThread implement
	- First we decide to have a ` struct ` data structure, where it stores ` TID
	, STATE, RETURN VALUE, CONTEXT, AND STACK ` 

	- [UTHREAD_CREATE()](https://github.com/zhongquanchen/ecs150proj2/blob/a91cac83b8121994c44e50d56bd17bd812bd6ccc/libuthread/uthread.c#L129)
	We first implement ` UTHREAD_CREAT() `, we first check if the main thread 
	has created, if not then we will call ` Initialize() `, which will create a
	thread to main. Then, it will create & assign value to the thread. Enqueue 
	it into ` READY STATE `. Then it will return the number of total thread if 
	created successfully. 

	- [UTHREAD_YIELD()](https://github.com/zhongquanchen/ecs150proj2/blob/a91cac83b8121994c44e50d56bd17bd812bd6ccc/libuthread/uthread.c#L54)
	When UTHREAD_YIELD() is called,it will dequeue the first ready state and 
	swap it with the previous state. 

	- [UTHREAD_EXIT()](https://github.com/zhongquanchen/ecs150proj2/blob/a91cac83b8121994c44e50d56bd17bd812bd6ccc/libuthread/uthread.c#L158)
	Will put the current thread into zombie state. Call the next thread from 
	ready queue. 

	- [UTHREAD_JOIN()](https://github.com/zhongquanchen/ecs150proj2/blob/a91cac83b8121994c44e50d56bd17bd812bd6ccc/libuthread/uthread.c#L191)
	Will set itself into block queue, and swap it with the thread it join with.
	It will wait until the child thread finish its jobs. When the child thread
	exit and become an zombie state, the parent thread will then collect the 
	value from child. 

## Phase 4 ##
- Preemption implement 
	- First we declare a static variable call ` static struct sigaction sa `
	This data structure will capture signal from time alarm. The signal name 
	is ` SIGVTALRM `

	- [PREEMPT_START(VOID)](https://github.com/zhongquanchen/ecs150proj2/blob/a91cac83b8121994c44e50d56bd17bd812bd6ccc/libuthread/preempt.c#L44)
	This function will set up a signal reciever and it will capture signal
	of VIRTUAL ALARM type. In the function, if there is a signal captured, 
	it will transfer to a signal handler, name ` TIMER_HANDLE(int sig_num) `function. 
	After that, we set an timer alarm for user level thread.
	This alarm will take the desire second to trigger signal. 
	
	During two create, we set up a failure return, in case the signal or
	alarm create fail.

	- [PREEMPT_ENABLE(VOID)](https://github.com/zhongquanchen/ecs150proj2/blob/a91cac83b8121994c44e50d56bd17bd812bd6ccc/libuthread/preempt.c#L36)
	This function will add a signal flag to the mask of the static vairable
	` sa `. When sa is initialize in ` preempt_start() ` method, the mask of
	sa is set to empty, which mean that it will not capture any signal.

	In this function, we add ` SIGVTALRM ` flag in sa's mask so that it can
	listen to the flag porperly.

	- [PREEMPT_DISABLE(VOID)](https://github.com/zhongquanchen/ecs150proj2/blob/a91cac83b8121994c44e50d56bd17bd812bd6ccc/libuthread/preempt.c#L28)
	The opposite of preempt_enable. It will delete the flag ` SIGVTALRM ` in
	sa' mask.

	- To make it work in uthread.c, we put [PREEMPT_START(VOID)](https://github.com/zhongquanchen/ecs150proj2/blob/a91cac83b8121994c44e50d56bd17bd812bd6ccc/libuthread/preempt.c#L44) in [UTHRE	AD_CREATE](https://github.com/zhongquanchen/ecs150proj2/blob/a91cac83b8121994c44e50d56bd17bd812bd6ccc/libuthread/uthread.c#L129) when the main thread is create. And enable it after they 
	have a context_switch in uthread in order to protect sharing data.

 	












