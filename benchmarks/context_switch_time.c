#define _POSIX_C_SOURCE 200809L //we need it because we are using clock_gettime() function whcih is a posix function
#include<stdio.h>
#include<stdint.h>
#include<time.h>
#include"thread.h"
#include"scheduler.h"
#define iterations 100000
static uint64_t get_time_ns(void){
    struct timespec ts;
    if(clock_gettime(CLOCK_MONOTONIC,&ts)!=0){ //The clock_gettime function will ask the os to give a timestamp.
        perror("clock_gettime");//used to print error msg when a function fails.
        return 0;
    }
    return (uint64_t)ts.tv_sec*1000000000ULL+ts.tv_nsec;//This will return total current time in nanoseconds.
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
    uint64_t start=get_time_ns();//recording starting timestamp.
    scheduler_dispatch();//start the scheduler and it selects ready threads and allow them to execute.
    uint64_t end=get_time_ns();//recording ending timestamp.    
    uint64_t elapsed=end-start;//Calculating the total time taken.
    
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