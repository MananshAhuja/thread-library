
#include "scheduler.h"
#include "thread.h"
#include <stdio.h>

void worker(void *arg)
{
    int id = *(int *)arg;

    for (int i = 1; i <= 5; i++)
    {
        printf("Worker thread %d: iteration %d\n", id, i);
       thread_yield();
 }
}

int main(void)
{
    thread_control_block thread1;
    thread_control_block thread2;

    int id1 = 1;
    int id2 = 2;

    if (thread_create(&thread1, worker, &id1) != 0)
    {
        printf("Thread 1 creation failed\n");
        return 1;
    }

    if (thread_create(&thread2, worker, &id2) != 0)
    {
        printf("Thread 2 creation failed\n");
        return 1;
    }

    printf("Both threads created successfully\n");

    scheduler_dispatch();

    printf("Program completed\n");
    return 0;
}

