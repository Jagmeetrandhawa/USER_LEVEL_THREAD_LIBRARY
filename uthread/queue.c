#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "queue.h"

/* Data Structure to Store Data into Queue Variables */
/* Front and End Nodes to Serve as the Front and Back Ends of the Linked List Implementation */
struct queue {
        int size;
        struct queue_nodes *front;
        struct queue_nodes *end;
};

/* Nodes to Store Entries to Queue for Linked List Implementation */
struct queue_nodes {
        struct queue_nodes* next;
        void *data;
};

/* Initializing the Queue for FIFO */
/* Allocate Memory for a New Queue */
/* Set pointers to Null or 0 to Signify Empty List to Allow Data to be Stored in the Next Steps */
queue_t queue_create(void)
{
        queue_t n_queue = malloc(sizeof(struct queue));

        n_queue -> size = 0;
        n_queue -> front = NULL;
        n_queue -> end = NULL;

        /* Return Newly Created Queue for FIFO Implementation */
        return n_queue; 
}

/* Deallocating Memory by Freeing Queue to be Destroyed from Program */
int queue_destroy(queue_t queue)
{
        /* Error Handling for Checking if Queue is Empty or not to Free Memory */
        if(queue == NULL || queue != NULL){
                return -1;
        }

        /* Free Queue to Deallocate Memory and Destroy Queue */
        free(queue);
        return 0; 
}

/* Adding New Data in our FIFO Queue */
int queue_enqueue(queue_t queue, void *data)
{
        /* Error Handling if Memory Allocation Failed */
        if (queue == NULL || data == NULL){
                return -1;
        }

        struct queue_nodes* temp = malloc(sizeof(struct queue_nodes));

        /* If Malloc Failed to Allocate Memory */
        if (temp == NULL){
                return -1;
        }

        /* Store Data into Temporary Queue */
        temp -> data = data; 

        /* If Queue is Empty, Then Both Nodes Point to the Same Spot */
        if (queue->size == 0){
                queue -> front = queue -> end = temp;
        }
        /* Shift Previous Rear Spot Over, then Proceed to make the New Node Rear */
        else{
                queue -> end -> next = temp;
                queue -> end = temp;
        }

        /* Increase Size of Queue After Enqueue Action */
        queue -> size++;
        return 0;
}

/* Switching Pointers to Corresponding Items after Dequeue */
int queue_dequeue(queue_t queue, void **data)
{
        struct queue_nodes *temp;
        *data = queue -> front -> data;

        /* Error Handling for Memory Allocation */
        if (queue == NULL || data == NULL){
                return -1;
        }

        /* Temporary Node is now the Front Node to be Dequeued */
        /* New Front Node is now the Next Node after the old First Node */
        temp = queue -> front;
        queue -> front = queue -> front -> next;

        /* Queue Size Decreases by One */
        /* Free/Deallocate and Remove the First Node that is Stored in Temp */
        queue -> size--;
        free(temp);
        return 0; 
}

/* Deeleting the Oldest Node within the Queue */
int queue_delete(queue_t queue, void *data)
{ 
        /* Error Handling for Empty Queue and Data */
        if (queue == NULL || data == NULL){
                return -1;
        }

        /* If Queue is Already Empty, Then no need to Delete */
        if (queue -> size == 0){
                return 0;
        }

        /* While Queue is Not Empty, Find Node to be Deleted, and Change Positions 
        of the Other Nodes in the Queue */
        struct queue_nodes *temp = queue -> front;
        while (queue->size != 0){
                /* If Temp Node holds Data Needed to be Deleted, Free that Node */
                if (temp != NULL && temp -> data == data){
                        queue -> front = temp -> next;
                        /* Delete Temp Node and Decrease Queue Size */
                        free(temp);
                        queue -> size--; 
                        return 0;
                }
                else if (temp != data){
                        /* Error Handling if Data Does not Exist */
                        if (data == NULL){
                                return -1;
                        }
                        /* Shift Front Node to Next Position to be New Temp Position */
                        temp = queue -> front;
                        queue -> front -> next = temp;
                        /* Decrease Queue Size */
                        queue -> size--;
                }
                /* Error Handling in Event Node Error */
                else{
                        return -1;
                }
        }
        return 0;
}

/* Looping Through Queue From Oldest Item to Newest */
int queue_iterate(queue_t queue, queue_func_t func, void *arg, void **data)
{
        /* Temporary Node Created as the Front */
        struct queue_nodes *temp = queue -> front;

        /* Error Handling if Queue or Func is Empty */
        if (queue == NULL || func == NULL){
                return -1;
        }

        /* If Data is Different than Null when Interation is Stopped Prematurely, 
        then Data Recieves the Data Item Where Queue Iteration was Stopped */
        while (temp != NULL){
                if(func(queue, data, arg) == 1){
                        if (data != NULL){
                                *data = temp -> data;
                        }
                        break; // Stop and Proceed to Check Other Nodes */
                }
                /* Move Temp Node to Next Position and Continue Iterating */
                temp = temp -> next;
        }
        return 0;
}

/* Return Queue Length */
int queue_length(queue_t queue)
{
        /* Error Handling if Queue was not Created */
        if (queue == NULL)
                return -1;
        else
                return queue -> size;
        
}
