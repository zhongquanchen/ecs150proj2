# REPORT #

## Phase 1 ##
- Queue implement
	- Detele 
	We implement the queue just like normal queue struct. For the 
	[queue_delete()] (), we compare the address of two value, if it points to
	the same address, then it will detele that pointer in the queue.

	- Iterate
	For [queue_iterate()](), we just iterate all the value in the queue and 
	return it to the pointer of ` (void**) retval ` 

## Phase 2&3 ##
- UThread implement
	- First we decide to have a ` struct ` data structure, where it stores ` TID
	, STATE, RETURN VALUE, CONTEXT, AND STACK ` 

	- [UTHREAD_CREATE()]()
	We first implement ` UTHREAD_CREAT() `, we first check if the main thread 
	has created, if not then we will call ` Initialize() `, which will create a
	thread to main. Then, it will create & assign value to the thread. Enqueue 
	it into ` READY STATE `. Then it will return the number of total thread if 
	created successfully. 

	- [UTHREAD_YIELD()]()
	When UTHREAD_YIELD() is called,it will dequeue the first ready state and 
	swap it with the previous state. 

	- [UTHREAD_EXIT()]()
	Will put the current thread into zombie state. Call the next thread from 
	ready queue. 

	- [UTHREAD_JOIN()]()
	Will set itself into block queue, and swap it with the thread it join with.
	It will wait until the child thread finish its jobs. When the child thread
	exit and become an zombie state, the parent thread will then collect the 
	value from child. 

## Phase 4
- NOT YET