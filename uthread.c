#include <assert.h>
#include <limits.h>
#include <signal.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#include "queue.h"
#include "preempt.c"
#include "private.h"
#include "uthread.h"

/* Macros for Implementation */
#define READY "ready"
#define RUNNING "running"
#define BLOCKED "blocked"
#define ZOMBIE "zombie"
#define EXITED "exited"

/* Data Structure to Store Data into Thread Variables */
struct TCB{
        int t_retval; /* Variable to for Return Value */
        uthread_t tid; /* Thread ID */
        uthread_ctx_t* context_queue; /* Context_Thread */
        void *stateOfThread; /* Thread State - Ready, Zombie, Blocked, Exited */
        void *SP; /* Stack Pointer for Threads */
};

/* Variable for Data to be Stored for Current Running Thread */
struct TCB* current_thread; 

/* Global Variables for Thread Implementation */
queue_t thread_queue, ready, running, zombie, blocked, exit_queue; 
uthread_t thread_increment = 0;

int uthread_start(int preempt)
{
        (void)preempt;

        /* Allocating Memory */
        struct TCB* main_thread = (struct TCB*)malloc(sizeof(struct TCB));
        uthread_ctx_t* context_thread = (uthread_ctx_t*)(malloc(sizeof(uthread_ctx_t)));

        /* Error Handling for Memory Allocation */
        if (main_thread == NULL){
                return -1;
        }
        if (context_thread == NULL){
                return -1;
        }

        /* Initializing Queues for Implementation */
        thread_queue = queue_create(); /* Main Thread */
        ready = queue_create();
        running = queue_create();
        zombie = queue_create();
        blocked = queue_create();

        /* Error Handling for If Thread is Empty */ 
        if (thread_queue == NULL){
                return -1;
        } 

        /* Registering Main Thread and Adding to Stack*/
        main_thread -> context_queue = context_thread;
        main_thread -> tid = thread_increment; /* TID = 0 */
        main_thread -> SP = uthread_ctx_alloc_stack();
        current_thread = main_thread;
        queue_enqueue(thread_queue, current_thread);
        queue_enqueue(running, current_thread);
       
        return 0;
}

/* Stopping the Thread Scheudling If There are no More User Threads */
int uthread_stop(void)
{
        struct TCB* saveThread = (struct TCB*)malloc(sizeof(struct TCB));
	saveThread = current_thread;

	queue_dequeue(running, (void**)&saveThread);
	queue_enqueue(exit_queue, saveThread);

        return 0;
}

/* Creating New User Thread with a New TID */
int uthread_create(uthread_func_t func)
{
        /* Memory Allocating */
        struct TCB* new_thread = (struct TCB*)malloc(sizeof(struct TCB));
        uthread_ctx_t* context_thread = (uthread_ctx_t*)(malloc(sizeof(uthread_ctx_t)));

        /* Error Handling for Memory Allocation */
        if (new_thread == NULL){
                return -1;
        }
        if (context_thread == NULL){
                return -1;
        }

        /* Creating New Thread and Registering this New Thread */
        thread_increment++;
        new_thread -> tid = thread_increment;
        new_thread -> context_queue = context_thread;
        new_thread -> SP = uthread_ctx_alloc_stack();
        new_thread -> stateOfThread = READY;
        uthread_ctx_init(new_thread ->context_queue, new_thread -> SP, func);
       

        queue_enqueue(thread_queue, new_thread);
        queue_enqueue(ready, new_thread);


        return new_thread -> tid; /* Return New TID for Thread */
}

/* Stops Current Thread Running to Allow Other Threads to Run */
void uthread_yield(void)
{
        /* Memory Allocation */
        struct TCB* saveThread = (struct TCB*)malloc(sizeof(struct TCB));
        saveThread = current_thread; /* Save Current Thread */
        struct TCB* getThread = (struct TCB*)malloc(sizeof(struct TCB));
	
        queue_dequeue(running, (void**)&saveThread);
        queue_enqueue(ready, saveThread);
        queue_dequeue(ready, (void**)&getThread); /* From Discusion Video 1,p2 */
		
        /* Now Current Thread is the Next Thread Needing to Run */
        current_thread = getThread;
        queue_enqueue(running, current_thread);
        /* Switch Between Executing Threads */
        uthread_ctx_switch(saveThread-> context_queue, current_thread -> context_queue);
}

/* Return Thread Identifier (TID) of Current Running Thread */
uthread_t uthread_self(void)
{
        return current_thread -> tid;
}

/* Exit from the Current Running Thread to End Execution */
void uthread_exit(int retval)
{
        /* Collecting the Return Value of Exiting Thread */
        /* Thread goes into a Zombie State */
	current_thread -> t_retval = retval;
	current_thread -> stateOfThread = ZOMBIE;

	queue_dequeue(running, (void**)&current_thread);
	queue_enqueue(exit_queue, current_thread);

        /* Memory Allocation */
	struct TCB* getThread = (struct TCB*)malloc(sizeof(struct TCB));

        /* Run New Thread if Needed */
	queue_dequeue(ready, (void**)&getThread);
	queue_enqueue(running, getThread);
	uthread_yield();
	
}


// Phase 3 Implementation
int uthread_join(uthread_t tid, int *retval)
{    
        /* Error Handling if TID not being Increased */
        if (tid == 0){
                printf("tid is 0 something wrong\n");
                return -1;
	}

        /* Infinite Loop for Phase 2 */
        while(true){

                /* If no Thread is Ready to Run */
                if(ready == 0){
                        break;
                }

                /* Thread is Blocked, Call Yield to Run Other Threads */
                current_thread -> stateOfThread = blocked;
		uthread_yield();

        }

	(void)retval;
        return 0;
}
