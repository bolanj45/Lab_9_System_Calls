/**
 * File: pmod.c
 * Author: Julian Bolan
 * Brief: Modifying processor priority via nice 
 *  and using nano sleep.
 * Date: 11/10/2026
 * Section: 121
 * Compile: make
 * Exe: pmod
 */

#define _GNU_SOURCE
#include <unistd.h>
#include <stdio.h>
#include <sched.h>
#include <stdlib.h>
#include <ctype.h>
#include "pinfo.h"
#include <sys/resource.h>
#include <signal.h>
#include <errno.h>
#include <time.h>

int main(void) {
    // man nice - the lower the nice value, the higher the favorbility / priority of the process to the system,
    // the higher the nice value, the lower the favorbility / priority of the process to the system
    // nice values range from -20 to 19

    // man 2 nice changes process priority

    // nanosleep(const struct timespec *req, struct timespec *rem) - high-resolution sleep
    // - sleeps for specified nanoseconds in req 
    // - rem stores remaining time left after interupt, can us NULL if this value isn't used.

    pid_t pid;
    int nice = 0;

    // changing priority
    pid = getpid();
    // pid = 8719;
    nice = getpriority(PRIO_PROCESS, pid);
    int newNice = nice + 10; 

    if (newNice <= 19) {
        printf("newNice %d\n", newNice);
        if (setpriority(PRIO_PROCESS, pid, newNice) == -1) { // prio of process, the process, new nice / prio value
            perror("setpriority errored\n");
        }
    } else {
        printf("newNice value %d is less than -20 and therfore is invalid\n", newNice);
        perror("newNice value is invalid\n");
    }

    struct timespec ts;
    ts.tv_sec = 1;
    ts.tv_nsec = 837272638; // 1837272638 is equal to 1.837... secs // which is out of the 0 to 999,999,999 range for tv_nsec
    // so then tv_sec = 1 and tv_nsec = 837272638
    nanosleep(&ts, NULL);
    printf("Goodbye\n");
    exit(0);

    return 0;
}

// when I attempted to decreased priority via a negative nice value I saw on htop that the 
// value for PRI changed to 30 // Note: I could not increase the priority.