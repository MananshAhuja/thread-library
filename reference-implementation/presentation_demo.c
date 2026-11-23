#define _POSIX_C_SOURCE 200809L
#include <stdint.h>
#include <stdio.h>
#include <time.h>
#include "thread.h"
#include "scheduler.h"
#include "mutex.h"
#include "semaphore.h"

static thread_mutex_t counter_lock;
static thread_sem_t item_ready;
static thread_control_block first_thread;
static int counter = 0;
static int item = 0;
static int errors = 0;

/* Try this part to see the work without our thread library.
   It is off now. Take away the comment marks to use it. */
/*
static void plain_worker(int number)
{
    for (int i = 0; i < 3; i++) {
        counter++;
        printf("Plain worker %d: counter = %d\n", number, counter);
    }
}

static void plain_demo(void)
{
    plain_worker(1);
    plain_worker(2);
    // One worker ends before the next worker starts.
}
*/

static void worker(void *arg)
{
    int number = *(int *)arg;
    for (int i = 0; i < 3; i++) {
        // One thread holds the lock at a time.
        if (thread_mutex_lock(&counter_lock) != 0) { errors++; return; }
        int old_value = counter;
        thread_yield(); // Give the other thread a turn.
        counter = old_value + 1;
        printf("Thread %d: counter = %d\n", number, counter);
        if (thread_mutex_unlock(&counter_lock) != 0) { errors++; return; }
        thread_yield(); // Let another ready thread run.
    }
}

static void maker(void *arg)
{
    (void)arg;
    item = 42;
    puts("Maker: the item is ready");
    if (thread_sem_post(&item_ready) != 0) errors++;
}

static void taker(void *arg)
{
    (void)arg;
    // Wait here until the maker gives us an item.
    if (thread_sem_wait(&item_ready) != 0) { errors++; return; }
    printf("Taker: I got %d\n", item);
    if (item != 42) errors++;
}

static void watcher(void *arg)
{
    (void)arg;
    // Wait until thread 1 is done.
    if (thread_join(&first_thread) != 0) { errors++; return; }
    puts("Watcher: thread 1 is done");
}

static uint64_t time_ns(void)
{
    struct timespec now;
    if (clock_gettime(CLOCK_MONOTONIC, &now) != 0) return 0;
    return (uint64_t)now.tv_sec * 1000000000ULL + (uint64_t)now.tv_nsec;
}

int main(void)
{
    thread_control_block second_thread, maker_thread, taker_thread, watcher_thread;
    int one = 1, two = 2;

    /* To see plain work, take away these comment marks too.
    plain_demo();
    counter = 0;
    */

    // Make one lock and one signal with no item yet.
    if (thread_mutex_init(&counter_lock) != 0 ||
        thread_sem_init(&item_ready, 0) != 0) return 1;

    // Make our five small threads.
    if (thread_create(&first_thread, worker, &one) != 0 ||
        thread_create(&second_thread, worker, &two) != 0 ||
        thread_create(&maker_thread, maker, NULL) != 0 ||
        thread_create(&taker_thread, taker, NULL) != 0 ||
        thread_create(&watcher_thread, watcher, NULL) != 0) return 1;

    puts("Start our threads");
    uint64_t start = time_ns();
    benchmark_switch_count = 0;
    int result = scheduler_run(); // Run the ready threads.
    uint64_t end = time_ns();

    printf("Final counter = %d (want 6)\n", counter);
    printf("Switches counted = %d\n", benchmark_switch_count);
    if (start && end >= start && benchmark_switch_count > 0) {
        printf("Time for this run = %llu ns\n", (unsigned long long)(end - start));
        printf("Average per counted switch = %.1f ns\n",
               (double)(end - start) / benchmark_switch_count);
    }
    // The average also counts the work done between switches.
    if (thread_mutex_destroy(&counter_lock) != 0 ||
        thread_sem_destroy(&item_ready) != 0) errors++;
    if (result != 0 || counter != 6 || errors) {
        puts("Demo did not pass");
        return 1;
    }
    puts("Demo passed");
    return 0;
}
