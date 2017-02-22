#define _GNU_SOURCE
#include <stdio.h>
#include <sched.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <time.h>
#include <limits.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <errno.h>
#include <sys/resource.h>
#include <sys/mman.h>
#include <arpa/inet.h>

char output[1000] = {0};

#define socketperror(format, ...) \
	sprintf(output, format, ##__VA_ARGS__); \
if (write(STDERR_FILENO, output, strlen(output)) == -1 || \
		fflush(stderr) < 0) \
perror("Error in writing to STDERR\n");

#define shrdmem_perror(format, ...) \
	sprintf(output, format, ##__VA_ARGS__); \
if (write(STDERR_FILENO, output, strlen(output)) == -1 || \
		fflush(stderr) < 0) \
perror("Error in writing to STDERR\n");

int global_socketid = 0;
typedef unsigned char uchar;
typedef unsigned int uint;
typedef struct timespec timespec;
typedef long long int longtime;
typedef long long int longint;


timespec global_res;
longtime global_time;

/* Optimized macro */
#define get_current_time() \
	({ longtime global_time = 0;\
	 clock_gettime(CLOCK_MONOTONIC, &global_res);\
	 global_time = (global_res.tv_sec * BILLION) + global_res.tv_nsec;\
	 global_time;\
	 })

// Buffer size - can tune the buffer size here
#define BUFF_SIZE (1024 * 64)
#define TRANSFER_SIZE (1024 * 1024 * 500)
#define BILLION (1000000000)

int write_full(int fd, void *buff, uint size) {
	int write_size = size;
	int rem = 0;
	while ((rem = write(fd, buff, write_size)) < write_size) {
		if (rem < 0) { return rem; }
		write_size -=rem;
		buff = buff + rem;
#if 0	
		printf("In loop. rem = %u, write_size = %u, size = %u\n", rem,
				write_size, size);
#endif
	}
	return size;
}

int read_full(int fd, void *buff, uint size) {
	int read_size = size;
	int rem = 0;
	while ((rem = read(fd, buff, read_size)) < read_size) {
		if (rem < 0) { return rem; }
		read_size -=rem;
		buff = buff + rem;
#if 0
		printf("In loop. rem = %u, read_size = %u, size = %u\n", rem,
				read_size, size);
#endif
	}
	return size;
}

