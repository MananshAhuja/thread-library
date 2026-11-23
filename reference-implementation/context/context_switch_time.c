#define _POSIX_C_SOURCE 200809L //we need it because we are using clock_gettime() function whcih is a posix function
#include<stdio.h>
#include<stdint.h>
#include<time.h>
#include"thread.h"
#include"scheduler.h"
#define iterations 100000
static int get_time_ns(uint64_t *time_ns)
{
    struct timespec ts;

    if (time_ns == NULL)
        return -1;

    if (clock_gettime(CLOCK_MONOTONIC, &ts) != 0)
    {
        perror("clock_gettime");
        return -1;
    }

    *time_ns = (uint64_t)ts.tv_sec * 1000000000ULL
               + (uint64_t)ts.tv_nsec;

    return 0;
}
//create a worker function
static void worker(void*arg)
{   (void)arg;
    for(int i=0;i<iterations;i++){
        thread_yield();
    }
}
int main(){
    thread_control_block threadA;
    thread_control_block threadB;
    thread_create(&threadA,worker,NULL);
    thread_create(&threadB,worker,NULL);
    //start measuring time
    benchmark_switch_count=0;
    uint64_t start, end;

if (get_time_ns(&start) != 0)
{
    fprintf(stderr, "Failed to get start timestamp\n");
    return 1;
}

scheduler_dispatch();

if (get_time_ns(&end) != 0)
{
    fprintf(stderr, "Failed to get end timestamp\n");
    return 1;
}

if (end < start)
{
    fprintf(stderr, "Invalid timer result\n");
    return 1;
}

uint64_t elapsed = end - start;
    uint64_t switches=benchmark_switch_count ;//stores number of switches counted.
    if(switches>0){
        double average=(double)elapsed/switches;
        printf("Average time per switch is %f",average);
    }
    printf("Context Swicth Benchmark\n");
    printf("Total number of iterations per thread is : %d\n",iterations);
    printf("The total elapsed time is %llu nano secs\n",(unsigned long long)elapsed);
    printf("Actual Switches Occured : %llu",(unsigned long long)switches);
}   