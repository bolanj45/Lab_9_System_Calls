/**
 * File: finfo.c
 * Author: Julian Bolan
 * Brief: printing file info.
 * Date: 11/10/2026
 * Section: 121
 * Compile: make
 * Exe: finfo
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
#include <sys/stat.h>
#include <fcntl.h>

// Major differences between fopen, fread, fwrite, and their system call conterparts open, read, write
// open had the ability to have flags as a parameter, and mode for permission bits of a newly created file compared to fopen
// read and write use number of bytes to read or write while fread and fwrite use nmemb which is the number of members (each member being size big)
int main(void) {

    char path[256];
    char buf[256];
    size_t bufSize = 256;

    printf("Enter filename / pathname to file: ");
    scanf("%s", path);

    int fd = open(path, O_RDWR); // O_RDWR stands for read and write
    if (fd == -1)
    {
        perror("Could not open file\n");
        exit(1);
    }
    
    ssize_t bytesRead = read(fd, buf, bufSize); // returns bytes read if successful, 0 at E0F (no bytes to be read), -1 on error 
    if (bytesRead == -1) {
        perror("Error reading file");
        exit(1);
    }
    struct stat fileStats; // struct provided by <sys/stat.h> and include stats about a file
    if (fstat(fd, &fileStats) == -1) { // fstat uses a file descriptor to find the file and sotres the data in the pointer to a stat struct
        perror("Couldn't retrive file information to store to fileStats");
        exit(1);
    }

    int permBits = fileStats.st_mode & 07777; // masking first 9 bits with Hex for normal permissions
    // and extra 3 bits for special permissions
    int fileType = fileStats.st_mode;

    if (S_ISREG(fileType)) {
        printf("File is regular\n");
    } else if (S_ISDIR(fileType)) {
        printf("File is not regular\n");
    } else if (S_ISLNK(fileType)) {
        printf("File is symbolic link\n");
    } else {
        printf("File type is unknown\n");
    }

    // char timeBuffer[256];
    struct tm *lastMod;

    if (localtime(&fileStats.st_mtime) != NULL) {
        lastMod = localtime(&fileStats.st_mtime);
    } else {
        perror("Couldn't convert fileStats mod time to local time\n");
        exit(1);
    }
    
    char modString[256];
    strftime(modString, 256, "%Y-%m-%d %H:%M:%S\n", lastMod); // string format time, taks lastMod time and converts to readable string 
    // and stored in char buffer with size indicated

    printf("Permission bits %04o\n", permBits); // octal since max value will be 7 for each permission set 
    // 0 can be out front indicating that the special permission bits are not shown if desired,
    // note that however special bit permissions are printed as well here.
    // note: chmod format

    printf("User id: %d\n", fileStats.st_uid);
    printf("Size of file in bytes %zu\n", fileStats.st_size);
    printf("Last modified time %s", modString);

    exit(0);
    return 0;
}