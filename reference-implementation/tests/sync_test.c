#include <stdio.h>
#include "thread.h"
#include "scheduler.h"
#include "mutex.h"
#include "semaphore.h"

enum { ITERATIONS = 10000 };
static thread_mutex_t lock;
static thread_sem_t empty, full;
static int shared, item, consumed, errors;

static void direct_increment(void) {
    for (int i = 0; i < ITERATIONS; ++i) ++shared;
}

static void increment(void *unused) {
    (void)unused;
    for (int i = 0; i < ITERATIONS; ++i) {
        if (thread_mutex_lock(&lock)) { ++errors; return; }
        int old = shared;
        thread_yield(); /* Force another thread to try the protected section. */
        shared = old + 1;
        if (thread_mutex_unlock(&lock)) { ++errors; return; }
    }
}

static void producer(void *unused) {
    (void)unused;
    for (int i = 1; i <= ITERATIONS; ++i) {
        if (thread_sem_wait(&empty)) { ++errors; return; }
        item = i;
        if (thread_sem_post(&full)) { ++errors; return; }
        thread_yield();
    }
}

static void consumer(void *unused) {
    (void)unused;
    for (int i = 1; i <= ITERATIONS; ++i) {
        if (thread_sem_wait(&full)) { ++errors; return; }
        if (item != i) ++errors;
        consumed++;
        if (thread_sem_post(&empty)) { ++errors; return; }
        thread_yield();
    }
}

int main(void) {
    thread_control_block a, b, p, c;
    direct_increment(); direct_increment();
    printf("Without thread library: two sequential calls, counter=%d\n", shared);
    shared = 0;
    if (thread_mutex_init(&lock) || thread_sem_init(&empty, 1) ||
        thread_sem_init(&full, 0) || thread_create(&a, increment, NULL) ||
        thread_create(&b, increment, NULL) || thread_create(&p, producer, NULL) ||
        thread_create(&c, consumer, NULL)) return 1;
    int status = scheduler_run();
    printf("With thread library: mutex counter=%d, semaphore transfers=%d, errors=%d\n",
           shared, consumed, errors);
    if (status || errors || shared != 2 * ITERATIONS || consumed != ITERATIONS ||
        thread_mutex_destroy(&lock) || thread_sem_destroy(&empty) ||
        thread_sem_destroy(&full)) return 1;
    puts("PASS: mutex and semaphore synchronization");
    return 0;
}
