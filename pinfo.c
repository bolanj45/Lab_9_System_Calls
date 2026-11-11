/**
 * File: pinfo.c
 * Author: Julian Bolan
 * Brief: 
 * Date: 11/10/2026
 * Section: 121
 * Compile: make
 * Exe: pinfo
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

int main(void) {

    pid_t pid;
    int nice = 0; // process priority value ranging from -20 to 19
    // lower value indicates higher priority 
    // 0 is the default value bc of getpriority()
    int pidSchedClass;
    char specifiedPID;

    
    do {
        
        printf("Specify pid? (y/N)\n");
        scanf("%c", &specifiedPID);
        printf("\n");

        if (!isalpha(specifiedPID) && (!('y' == specifiedPID) || !('N' == specifiedPID))) {
            printf("Enter letter y/N (case sensitive)\n");
        }

    } while(!isalpha(specifiedPID) && (!('y' == specifiedPID) || !('N' == specifiedPID)));

    if ('y' == specifiedPID) {
        printf("Enter pid: ");
        scanf("%d", &pid); // only has pidExists right now
        printf("\n");
    }

    if (('y' == specifiedPID) && pidExists(pid)) {
        nice = getpriority(PRIO_PROCESS, pid); // 0 is the default value
        // PRIO_PROCESS - determines how the pid will be interpreted (which) (says who is a process id here)
        // pid to find the priority of (who)
        // getpriority(int which, id_t who);
        printf("PID priority: %d\n", nice);

        pidSchedClass = sched_getscheduler(pid); // returns the schedule policy / scheduler class of a process that handles the process
        
        if (pidSchedClass != -1) {
            getSchedClass(pidSchedClass);
        } else {
            perror("Couln't fetch PID schedule class\n");
        }

    } else if ('N' == specifiedPID) {
        printf("Printing current caller ID\n");
        pid = getpid(); // return caller's pid (current working process' pid) // doesn't return a error value or set errno

        printf("Current executing process's ID: %ld\n", (long) pid);
        nice = getpriority(PRIO_PROCESS, pid);
        printf("PID priority: %d\n", nice + 20);
        // Note that PRI = 20 + NI, PRI is the priority, NI is the nice value which alters the priority

        pidSchedClass = sched_getscheduler(pid);
        if (pidSchedClass != -1) {
            getSchedClass(pidSchedClass);
        } else {
            perror("Couln't fetch PID schedule class\n");
        }
    }
    
    else {
        perror("Entered pid DNE\n");
        printf("Terminating Program\n");
        exit(1); // 1 - exit on failure
    }

    return 0;
}


int pidExists(pid_t pid) {
    if (kill(pid,0) == 0) {
        return 1; // process exists
    }
        return errno == EPERM; // 1 indicates that the pid exists, but you do not have access to use it
        // 0 indicates process doesn't exists or can't be used check errno
        // errno equals ESRCH indicates that process does not exist
}

void getSchedClass(int schedPolicy) {
    switch(schedPolicy) {

        case SCHED_OTHER: // Class: Normal time sharing - Default scheduling class, priority is determined by nice value
        puts("SCHED_OTHER"); 
        break;

        case SCHED_FIFO: // Class: Real-time - Runs until a higher priority task exists and is stalled until that higher priority task finished
        puts("SCHED_FIFO");
        break;

        case SCHED_RR: // Class: Realtime - Similar to FIFO, however equal-priority tasks take equal intermittent times in taking turns running
        puts("SCHED_RR");
        break;

        case SCHED_BATCH: // Class: Linux normal class - used for CPU bound non-interactive work, tends to not interrupt other processes to run *this* process
        puts("SCHED_BATCH");
        break;

        case SCHED_IDLE: // Class: Linux low priority - Only runs when no other tasks wants to utilize the CPU
        puts("SCHED_IDLE");
        break;

        case SCHED_DEADLINE: // Class: Linux Deadline - Runs on the earliest-deadline-first with runtime/period/deadlineline parameters set with sched_setattr();
        puts("SCHED_IDLE");
        break;

        default: // Used for uncommon policies
        printf("Schedule policy: %d\n", schedPolicy);
    }
}

// int getcpu(unsigned *cpu, unsigned *node) states which CPU core and (NUMA node which is for Non-uniform Memory Access) the calling thread is running currently
// cpu - cpu number (can be NULL)
// node - NUMA node number (can also be NULL)

// getrusage(int who, struct rusage *usage); // reports resource usage statistics for a process such as CPU time, memory, page faults, context switches and more
// page faults occur if the program goes to a memory address where there isn't a memory page currently
// A context switch is when the CPU switches running one thread to another
// A thread is a single path/line of execution within a process

// getrlimit(int resource, struct rlimit *rlim); // reads the resource limits of the calling process which are defined by limits the kernel has on items like memory, open files, CPUE time, and stack size.
// setrlimit(int resource, const struct rlimit *rlim); // changes the limits enforced by the kernel

