/**
 * File: info.c
 * Author: Julian Bolan
 * Brief: Retrieving and printing realtime and systeminfo in a human-readable format
 * Date: 11/10/2026
 * Section: 121
 * Compile: make
 * Exe: info
 */

#include <sys/sysinfo.h>
#include <sys/utsname.h>
#include <unistd.h>
#include <time.h>
#include <stdio.h>

int main(int argc, char* argv[]) {

    struct timespec ts; // stores seconds plus nanoseconds
    time_t seconds_since_midnight;  // time_t stores a point in time as a single number 
    // seconds since midnight is the same as seconds since new day

    struct sysinfo info; // struct that contains info relative to the system
    struct tm* local_time; // stores time broken down into calendar members / fields / variables

    clock_gettime(CLOCK_REALTIME, &ts); // stores realtime in seconds and nanoseconds to the timespec struct ts
    local_time = localtime(&ts.tv_sec); // local time will convert calendard real time to local and expects the seconds member of the timespec struct 
    
    seconds_since_midnight = local_time->tm_hour*3600 + local_time->tm_min*60 + local_time->tm_sec; 
    // converting the local time calendar members of the tm struct to seconds and adding all the seconds up

    // printf("Time of day in nano seconds: %d\n", seconds_since_midnight);


    // timeInNanoSec = ts.tv_sec * pow(10,9) + 


    printf("Realtime in seconds with nano second resolution:  %lu.%09lu\n", seconds_since_midnight, ts.tv_nsec);

    // char buffer[256];
    struct utsname buffer;
    if (uname(&buffer) == 0) { // reading in system, kernel, and hardware info etc. and storing to utsname struct buffer

        printf("system name %s\n", buffer.sysname); 
    
        printf("network host name: %s\n", buffer.nodename); // prints hostname

        printf("Operating system release: %s\n", buffer.release);

        printf("Operating system version: %s\n", buffer.version);
    
        printf("hardware type: %s\n", buffer.machine);

    } else {
        perror("Could not fetch uname\n");
    }
    sysinfo(&info); // stores system-wide statistics i.e. uptime, load everages,
    // memory, swap usage (partition or file that the system overwrites as overlow for RAM),
    // and process count
    
    
    
    char name[256];
    if (gethostname(name, 256) == 0) {
        printf("Host name: %s\n", name);
    } else {
        perror("Couldn't fetch hostname\n");
    }
    
    int processors = get_nprocs(); // number of processes
    printf("Number of processors available: %d\n", processors);

    // convert to bytes
    
    long page = sysconf(_SC_PAGESIZE); // returns the memory page in bytes
    // could also use
    // int page = getpagesize();
    if (page == -1) {
        perror("Couldn't fetch pagesize\n");
    } else {
        // printf("page bytes: %lu", page);
    }

    
    long pages = sysconf(_SC_PHYS_PAGES);
    if (pages == -1) {
        perror("Couldn't fetch number of pages\n");
    } else {
        // printf("number of pages\n", pages) // total physical pages (Linux here)
    }

    if(!((page == -1) && (pages == -1))) {
        printf("Total amount of physical memory in bytes: %lu\n", pages * page);
    } 
    
    
    long freePage = sysconf(_SC_AVPHYS_PAGES); // current free physical pages 
    if ((freePage == -1) && (pages != -1)) {
        printf("Total amount of free memory in bytes %lu\n", pages*freePage);
    }
    
    return 0;
}

