# **Project 2: User Library Thread**
***
## **Summary of Project 2**

The goal of this project is to be able to create a queue alogrithm and build upon that idea to implement and create a multithread user-level library. This project is to allow us to learn and understand the idea of threads by building a basic user-level thread library in C. As a group, we decided to use a linked list implementation to achieve a FIFO queue algorithm that has operations running in O(1) time. 

***
## **Queue Implementation**

In order to implement a simple FIFO queue, we decided to go with a linked list implementation in order to have all operations, apart from interate and delete, to be running in O(1) time. In order to accomplish this task, we created two `struct` data structures to store the necessary data needed for enqueueing, dequeueing, deleting, and destroying items within our queue.

### **Struct Queue and Queue_nodes**

The purpose of creating two separate structures for a linked list implementation is to store necessary data items our queue receives and have that data stored in order to be used in necessary operations such as enqueueing new items, deleting items etc. 

#### **Struct Queue_nodes**
Creating a linked list in this structure in order to be able to implement a linked list for our FIFO queue.
- A pointer named `next` in order to hold the address of the next node and creates a link between two nodes. We use this to point our `front` node to the next position if a item is deleted or added to the queue. 
- `void data` to hold addresses and items within our queue. 

#### **Struct Queue**
- `int size` to obtain the length of the queue at any moment within the queue. Will update as new items are added into the queue or removed. `int size` returns empty if no queue has been created. 
- Nodes `front` and `end` to hold the addresses of items located at the front and back of our queue. We point these nodes to the respective items if necessary if we encounter items being added or removed in the queue. We mainly use these nodes in enqueueing, dequeueing, and deleting items from the queue. 

### **Queue_Create and Queue_Destroy**

When creating a queue in `queue_create()`, we focus on allocating memory for the queue by using `malloc` to ensure we have the appropriate amount of memory necessary for any amount of items that may be inserted into the queue. Once our queue is created we.... 
- Initialize the `size` of our queue as 0 to state a new empty queue was created.
- Point `front` node as NULL as well as `end` node as NULL to signify that no items currently exist within our empty queue. 
- Return our newly created queue to be ready for items to be stored in.

When we no longer have a need for the queue, we call `queue_destroy()` to deallocate the memory we originally created and `return 0` if destroying was successful. 
- We implemented error handling to check if a queue still exists when `queue_delete()` is called to ensure our queue code does not crash. `return -1` if an error occurs.
- We use `free()` to deallocate memory that was obtained when making the queue. 

### **Queue_Enqueue**

The operation of enqueueing a new item into the queue is adding the item to the end of the queue. In order to implement enqueue we... 
- Created a temporary node called `temp` to assign the value of the `data` item that was enqueued. 
- Created error handling to return a -1 if memory allocation errors have ocurred for `queue`, `*data`, and `temp`. 
- Checked if the queue is currently empty, we point our nodes `front` and `end` to our temporary node `temp` to signify this `temp` node is the only existing item in the queue. We do `queue -> front = queue -> end = temp` to do this. 
- Checked that if queue is not empty and other items are already in the queue, we point `end` node to the new node `temp` and make `temp` the new `end` node. We do this to say this new item is now the last item of our queue and will be the last one to leave the queue. We do `queue -> end -> next = temp;` and `queue -> end = temp;` to do this operation. 
- We increase the size of our queue, `queue -> size++;`, to reflect that a new item has joined the queue.

### **Queue_Dequeue**

The operation of dequeueing is to do the opposite of enqueue, by deleting the first item that is in the queue and make the following item the new front node. To implement this operation we ....
- Created a temporary node called `temp` to assign the value of the `data` item that is currently at the front of the queue.
- Created error handling to check if memory allocation encountered an error or no data is currently present in queue.
- Assign the `temp` node as the `front` node in the queue and point the `front` node to the `next` item (second item) in the queue. We do this by writing `temp = queue -> front;` and `queue -> front = queue -> front -> next;`.
- We then `free()` the `temp` node that has the value of the first item in the queue to remove it. Then proceed to decrease queue size by one `queue -> size--;` to reflect the operation of dequeueing has occurred.

### **Queue_Delete**

Operation of deleting is being able to find the node we want to delete without affecting the other nodes located within the queue. Our implemention was the following:
- Error handling for memory allocation, `return -1` if queue or data is NULL.
- We checked if our queue is already empty, to return 0 as completed to avoid running this operation if no items exist in the queue. We did `if(queue -> size == 0) return 0;` to reflect our idea. 
- Created a `temp` node, and using a `while` loop to iterate through the queue while it is not empty to find the node that needs to be deleted. 
- When `temp` node is the value needed to be deleted, shift the `front` node to the next position, and decrease queue size. 
- Created another error handling at the end of the while loop to catch if any node errors were created to `return -1`. 

### **Queue_Iterate**
Interating through the queue to assign item values into the queue as new items are inserted. We use the `temp` node to assign the value of an item, insert it into the queue, and move along as new items are present in the queue. For implementation we did ....
- A `while` loop to move along the queue as long as our `temp` node is not empty. 
- We prematurely stop the iteration process by checking if func is equal to 1 by doing `if(func(queue,data,arg)==1))` and then check if we have data waiting to be added into the queue.
- Assigning the `*data` value item to our `temp` node, move `temp` into the next queue position, and proceed to iterate the queue again. 

### **Queue_Length**

Our last operation within our queue implementation is finding the queue length. In order to obtain our queue size, we have the varaible `int size` that we have stored within our `struct queue` that stores the queue length as we do operations to the queue. As we have `int size` in our struct, for this operation we simply check if the queue is NULL to return a -1 and if not, we return the queue length by doing `return queue -> size` to reflect the length of the queue while operations are occurring. 

***

## **User Thread Implementation**

In order to implement a user-level multithread library, we use resources from the man pages to help direct us in the right direction. We used `macros`, `structs`, and `global variables` to help us with our implementation. 

### **Uthread_Start**

In this operation, we went ahead to allocate memory, initialize queues for implementation and register the main thread in order to be ready for the following operations to use when needed. 
- Allocated memory for our `main_thread` and `context_thread`; Proceeded to do error handling to check if memory allocation has failed.
- Initialzing queues for implementation by using the process states we saw in lecture. We initialized queues for the `thread_queue, ready, running, zombie, and blocked states`. 
- We checked if `queue_create()` failed by doing error handling on only one of the initializing queues we did. We thought that if `thread_queue` recieves an error, then the other queue initializations have also failed. 
- Registering our main thread by pointing to our created `context_queue`, pointing to `tid` that has the value of `0`, and pointing to our stack that calls `uthread_ctx_alloc_stack()`. 

### **Uthread_Create**

Similar to our `uthread_start()` operation, we are allocating memory, error handling, and proceed to register our memory allocated thread to be ready to run as a thread in our multithread library process. 
- Allocated memory for our `new_thread` and `context_thread`; Proceeded to do error handling to check if memory allocation has failed. We check for errors by checking if either thread is NULL to `return -1`.
- Registering our `new_thread` by pointing to `tid`, which has been increased by 1 to represent as the new thread. We did `thread_increment++` to now make `tid = 1` for this first `new_thread`. 
- We use our variable `stateOfThread` from our `struct` to register our `new_thread` as the thread that is `ready` to run to follow the process state diagram we saw in lecture to run until it reaches a zombie state when done executing. 
- We proceed to return our `new_thread` with its corresponding thread identifier `tid` to let the program know which thread has just been created is will be now running in the multithread library. 

### **Uthread_Yield**

In this operation, we are calling the current thread to stop running and to let other threads to run. In order to make this work, we ....
- Allocated memory to threads called `saveThread` and `getThread`. We then proceeded to make `saveThread = current_thread` to save the current thread that is running. 
- We then do `current_thread = getThread` to make the new current thread to be the next thread that is ready to run. 
- We call `uthread_ctx_switch()` to switch threads and allow other threads to now be executed and run. 

### **Uthread_Join**

This operation makes the current thread wait for the `tid` to be complete and assign the return value of the finished thread. We then have the thread be joined by another thread.
- In our implementation, we did a error handling to check if tid has failed to update. We did `if(tid == 0) return -1` to print and let us know if the thread identifier has failed to update. 
- We have a while loop running to make the thread go into the `blocked` state and call `uthread_yield()` to have another thread join. 

### **Uthread_Stop and Uthread_Exit**

Both operations are similar. For stop, we want to stop the multithreading library completely if there are no other threads to be run. In exit, we are simply exiting the current running thread and waiting to finish its execution. 

#### **Uthread_Stop**
- We allocate memory into a thread called `saveThread` and make it equal to the current running thread. `saveThread = current_thread`. 
- We dequeue the running thread and proceed to enqueue the exit thread accomplish the task of stopping the multithreading library process.

#### **Uthread_Exit**
- We grab the return value of the current running thread by doing `current_thread -> t_retval = retval` and assign the current thread to a zombie state to exit from the running and blocked state. We do so by saying `current_thread -> stateOfThread = ZOMBIE` and proceed to dequeue. 
- In case there are more threads to be run, we dequeue the thread that is ready to run, enqueue it to the running thread state and then we call `uthread_yield()` to run any other threads that need to be executed as we exit and finish this current thread. 

### **Uthread_Self**
When running these threads, we are collecting their thread identifiers, `TIDs`, and storing the value in the `struct` that we have created to keep it stored. In this operation, we simply return the value of the thread identifier for the corresponding thread that is running and has finished. 
- To return the thread identifier `TID`, we did `return current_thread -> tid` to send the value of the thread identifier of the current thread.
- `TID` is updated if other threads need to be run. Thread identifier is updated in the `uthread_create()` operation if another thread is created and will be ready to run. 

***
## **Testing**
For our queue and user-level thread implementation, we built upon the `queue_tester_example.c` file we were given and created a `queue_tester.c` file with additional tests to make sure our queue was working. We decided to do few tests to check if multiple data items were able to be stored, if we can find an item within the queue by iterating, does enqueue and dequeue work, and finally checking if queue can be successfully destroyed. 

For user-level thread, we implemented a bit of error handling within our code to check if memory allocation did not work, as well as checking if our thread identifier variable `tid` was updating and working. We also utilized the given files `uthread_hello.c` and `uthread_yield.c` to check if threads were able to be created and stopped. 

Another helpful tool that we used was from the `Debugging Discussion Slides` that were presented to us. We used `gdb` on the `CSIF` machines and utilized `backtrace` to help us find errors we were encountering whilst working on the user-level multithread library concerning malloc and other features.

***
## **References for Project**

- Lecture Slides, Lecture Code, and Discussion Slides 
    - Specifically Discussion Slides for Makefile and Debugging
- Using GeeksforGeeks linked list implementation of FIFO to understand how it works and understanding how Enqueuing and Dequeueing plays a role in this algorithm:
    https://www.geeksforgeeks.org/queue-linked-list-implementation/
- Using GeeksforGeeks to learn how to create a deleting function for the queue: 
    https://www.geeksforgeeks.org/linked-list-set-3-deleting-node/
- Helping with implementation of enqueueing and dequeueing in a FIFO queue:
    https://www.codesdope.com/blog/article/making-a-queue-using-linked-list-in-c/
- Man Pages to help with implementation and understand how a multithread library works in C 
    - pthread_create() Man Page:
    https://man7.org/linux/man-pages/man3/pthread_create.3.html 
    - pthread_join() Man Page:
        https://man7.org/linux/man-pages/man3/pthread_join.3.html
    - pthread_exit() Man Page:
        https://man7.org/linux/man-pages/man3/pthread_exit.3.html

