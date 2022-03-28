#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "queue.h"

#define TEST_ASSERT(assert)                             \
do {                                                                    \
        printf("ASSERT: " #assert " ... ");     \
        if (assert) {                                           \
                printf("PASS\n");                               \
        } else  {                                                       \
                printf("FAIL\n");                               \
                exit(1);                                                \
        }                                                                       \
} while(0)

/* Create Queue */
void test_create(void)
{
        fprintf(stderr, "*** TEST create ***\n");

        queue_t q = queue_create();
        TEST_ASSERT(queue_create() != NULL);
        TEST_ASSERT(queue_length(q) == 0);
}

/* Enqueue/Dequeue simple */
void test_queue_simple(void)
{
        int data = 3, *ptr;
        queue_t q;

        fprintf(stderr, "*** TEST queue_simple ***\n");

        q = queue_create();
        queue_enqueue(q, &data);
        queue_dequeue(q, (void**)&ptr);
        TEST_ASSERT(ptr == &data);
}

/* Test Error Handling for Queue FIFO */
void test_error(void)
{
        queue_t q;
        int *data = NULL, temp;
        q = NULL;

        fprintf(stderr, "*** TEST queue_error ***\n");

        TEST_ASSERT(queue_length(q) == -1);

        temp = queue_enqueue(q, &data);
        TEST_ASSERT(temp == -1);

        temp = queue_delete(q,&data);
        TEST_ASSERT(temp == -1);
}

/* Checking if Queue can Handle Multiple Data Items */
void test_multiple_elements(void)
{
        int data = 3, *ptr;
        int mData = 4, *Mptr;
        queue_t q;

        fprintf(stderr, "*** TEST queue_multiple elements ***\n");

        q = queue_create();
        queue_enqueue(q, &data);
        queue_enqueue(q, &mData);
        queue_dequeue(q, (void**)&ptr);
        queue_dequeue(q, (void**)&Mptr);

        TEST_ASSERT(ptr == &data);
}

/* Callback Function that Finds a Certain Item According to its Value */
static int find_num(queue_t queue, void *data, void *arg)
{
        /* From Project 2 Phase 1 Prompt for Iteration */
        queue_t q;
        int temp = queue_dequeue(queue, &data);
        int match = 15;
        (void)arg; //unused
        (void)q;
        (void)temp;

        if (*(int*)data == match){
                return 1;
        }
        else{
                return 0;
        }
}

/* Checking if Items can be Added to Queue and Find the Item We Want in Queue */
void test_iterator(void)
{
        queue_t q;
        int *ptr;
        int data = 1, data_2 = 2, data_3 = 15, data_4 = 4, data_5 = 42;


        /* Initialize the queue and enqueue items */
        fprintf(stderr, "*** TEST queue_iterator ***\n");

        q = queue_create();

        queue_enqueue(q,&data);
        queue_enqueue(q,&data_2);
        queue_enqueue(q,&data_3);
        queue_enqueue(q,&data_4);
        queue_enqueue(q,&data_5);

        queue_iterate(q, find_num, NULL, (void**)&ptr);

        TEST_ASSERT(ptr == &data_3);
}

/*Test queue_delete */
void test_queue_delete(void){
        int data = 3, data_2 = 4, data_3 = 10, data_4 = 15, data_5=42, delete, *ptr;
        queue_t q;

        fprintf(stderr, "*** TEST queue_delete ***\n");

        q = queue_create();
        queue_enqueue(q, &data);
        queue_enqueue(q, &data_2);
        queue_enqueue(q, &data_3);
        queue_enqueue(q, &data_4);
        queue_enqueue(q, &data_5);

        /* Remove Item from Queue */
        queue_delete(q,&data);
        queue_dequeue(q, (void**)&ptr);
        queue_dequeue(q, (void**)&ptr);

        TEST_ASSERT(ptr == &data_3);
        TEST_ASSERT(queue_length(q) == 2);

        /* Delete Next Two Items From Queue */
        queue_dequeue(q, (void**)&ptr);
        queue_dequeue(q, (void**)&ptr);

        TEST_ASSERT(ptr == &data_5);
        TEST_ASSERT(queue_length(q) == 0);

        /* Queue no Longer Needed / Delete Entire Queue */
        delete = queue_destroy(q);
        TEST_ASSERT(delete == -1);
}

/* Call Testing Functions */
int main(void)
{
        test_create();
        test_queue_simple();
        test_multiple_elements();
        test_iterator();
        test_queue_delete();
        test_error();

        return 0;
}
