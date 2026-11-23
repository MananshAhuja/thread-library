#include <chrono>
#include <iostream>
#include "thread.hpp"

extern "C" {
#include "scheduler.h"
#include "mutex.h"
#include "semaphore.h"
}

static thread_mutex_t counter_lock;
static thread_sem_t item_ready;
static Thread *first_thread = nullptr;
static int counter = 0;
static int item = 0;
static int errors = 0;

/* Try this part to see the work without our thread library.
   It is off now. Take away the comment marks to use it. */
/*
static void plain_worker(int number)
{
    for (int i = 0; i < 3; i++) {
        ++counter;
        std::cout << "Plain worker " << number << ": counter = " << counter << '\n';
    }
}

static void plain_demo()
{
    plain_worker(1);
    plain_worker(2);
    // One worker ends before the next worker starts.
}
*/

static void worker(void *arg)
{
    int number = *static_cast<int *>(arg);
    for (int i = 0; i < 3; ++i) {
        // One thread holds the lock at a time.
        if (thread_mutex_lock(&counter_lock) != 0) { ++errors; return; }
        int old_value = counter;
        Thread::yield(); // Give the other thread a turn.
        counter = old_value + 1;
        std::cout << "Thread " << number << ": counter = " << counter << '\n';
        if (thread_mutex_unlock(&counter_lock) != 0) { ++errors; return; }
        Thread::yield(); // Let another ready thread run.
    }
}

static void maker(void *)
{
    item = 42;
    std::cout << "Maker: the item is ready\n";
    if (thread_sem_post(&item_ready) != 0) ++errors;
}

static void taker(void *)
{
    // Wait here until the maker gives us an item.
    if (thread_sem_wait(&item_ready) != 0) { ++errors; return; }
    std::cout << "Taker: I got " << item << '\n';
    if (item != 42) ++errors;
}

static void watcher(void *)
{
    // Wait until thread 1 is done.
    if (first_thread->join() != 0) { ++errors; return; }
    std::cout << "Watcher: thread 1 is done\n";
}

int main()
{
    int one = 1, two = 2;

    /* To see plain work, take away these comment marks too.
    plain_demo();
    counter = 0;
    */

    // Make one lock and one signal with no item yet.
    if (thread_mutex_init(&counter_lock) != 0 ||
        thread_sem_init(&item_ready, 0) != 0) return 1;

    // C++ makes the threads with short, clear names.
    Thread first(worker, &one);
    Thread second(worker, &two);
    Thread producer(maker);
    Thread consumer(taker);
    Thread observer(watcher);
    first_thread = &first;

    if (first.start() != 0 || second.start() != 0 ||
        producer.start() != 0 || consumer.start() != 0 ||
        observer.start() != 0) return 1;

    std::cout << "Start our threads\n";
    benchmark_switch_count = 0;
    auto start = std::chrono::steady_clock::now();
    int result = scheduler_run(); // Run the ready threads.
    auto end = std::chrono::steady_clock::now();
    auto nanoseconds = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();

    std::cout << "Final counter = " << counter << " (want 6)\n";
    std::cout << "Switches counted = " << benchmark_switch_count << '\n';
    if (benchmark_switch_count > 0) {
        std::cout << "Time for this run = " << nanoseconds << " ns\n";
        std::cout << "Average per counted switch = "
                  << static_cast<double>(nanoseconds) / benchmark_switch_count << " ns\n";
    }
    // The average also counts the work done between switches.
    if (thread_mutex_destroy(&counter_lock) != 0 ||
        thread_sem_destroy(&item_ready) != 0) ++errors;
    if (result != 0 || counter != 6 || errors != 0) {
        std::cout << "Demo did not pass\n";
        return 1;
    }
    std::cout << "Demo passed\n";
    return 0;
}
