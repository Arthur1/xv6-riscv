// time.c for xv6 (assignment #1 for CSC.T371)
// name: <<<your name>>>
// id: <<<your student id>>>

#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/time.h"
#include "user/user.h"

#define FD_STDOUT 0
#define FD_STDERR 1
#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(FD_STDERR, "usage: %s command [arg...]\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // time value before starting
    struct timeval start_time;
    gettimeofday(&start_time);

    int child = fork();
    if (child < 0) {
        fprintf(FD_STDERR, "%s: cannot fork\n", argv[0]);
        exit(EXIT_FAILURE);
    } else if (child == 0) {
        // child
        exec(argv[1], argv + 1);
    } else {
        // parent
        if (wait(0) < 0) {
            fprintf(FD_STDERR, "%s: cannot wait\n", argv[0]);
            exit(EXIT_FAILURE);
        }
    }

    // NOTE: float can't be used
    struct timeval end_time;
    gettimeofday(&end_time);
    uint64 diff_time_sec = end_time.tv_sec - start_time.tv_sec;
    uint64 diff_time_100msec = (end_time.tv_usec - start_time.tv_usec) / 100000;
    if (diff_time_100msec < 0) {
        diff_time_100msec += 10;
        diff_time_sec--;
    }

    // print
    printf("%l.%ls total\n", diff_time_sec, diff_time_100msec);
    exit(EXIT_SUCCESS);
}
