#define _POSIX_C_SOURCE 200809L
#include <stdint.h>
#include <stdio.h>
#include <time.h>
#include "thread.h"
#include "scheduler.h"

enum { ITERATIONS = 100000 };
static volatile unsigned long work;
static void direct_work(void) {
    for (int i = 0; i < ITERATIONS; ++i) ++work;
}
static void worker(void *unused) {
    (void)unused;
    for (int i = 0; i < ITERATIONS; ++i) {
        ++work;
        thread_yield();
    }
}
static uint64_t now_ns(void) {
    struct timespec ts;
    if (clock_gettime(CLOCK_MONOTONIC, &ts)) return 0;
    return (uint64_t)ts.tv_sec * 1000000000ULL + (uint64_t)ts.tv_nsec;
}
int main(void) {
    thread_control_block a, b;
    uint64_t start = now_ns();
    direct_work(); direct_work();
    uint64_t baseline = now_ns() - start;
    if (thread_create(&a, worker, NULL) || thread_create(&b, worker, NULL)) return 1;
    benchmark_switch_count = 0;
    start = now_ns();
    int status = scheduler_run();
    uint64_t elapsed = now_ns() - start;
    int switches = benchmark_switch_count;
    printf("Without thread library: %llu ns for %d sequential increments\n",
           (unsigned long long)baseline, 2 * ITERATIONS);
    printf("With thread library: %llu ns, %d recorded switches, %.2f ns per recorded switch\n",
           (unsigned long long)elapsed, switches,
           switches ? (double)elapsed / switches : 0.0);
    puts("Timing includes loop work, scheduler decisions and context switches; it is not pure switch latency.");
    return status || !start || !switches || work != 4UL * ITERATIONS;
}
