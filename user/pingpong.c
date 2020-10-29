// pingpong.c for xv6 (assignment #1 for CSC.T371)
// name: <<<your name>>>
// id: <<<your student id>>>

#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define FD_STDOUT 0
#define FD_STDERR 1
#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1
#define KEY_READ 0
#define KEY_WRITE 1

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(FD_STDERR, "usage: %s N\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // # of rounds
    int n = atoi(argv[1]);

    // tick value before starting rounds
    int start_tick = uptime();

    int pipe_fd_ptoc[2];
    int pipe_fd_ctop[2];
    if (pipe(pipe_fd_ptoc) < 0) {
        fprintf(FD_STDERR, "%s: cannot open pipe\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    if (pipe(pipe_fd_ctop) < 0) {
        fprintf(FD_STDERR, "%s: cannot open pipe\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int child = fork();
    if (child < 0) {
        fprintf(FD_STDERR, "%s: cannot fork\n", argv[0]);
        exit(EXIT_FAILURE);
    } else if (child == 0) {
        // child
        close(pipe_fd_ptoc[KEY_WRITE]);
        close(pipe_fd_ctop[KEY_READ]);
        uchar value;

        for (int round = 0; round < n; round++) {
            // Read: parent -> child
            while (read(pipe_fd_ptoc[KEY_READ], &value, 1) < 0);
            // fprintf(FD_STDOUT, "%d\n", value); // debug code

            // Write: child -> parent
            value++;
            write(pipe_fd_ctop[KEY_WRITE], &value, 1);
        }
        close(pipe_fd_ptoc[KEY_READ]);
        close(pipe_fd_ctop[KEY_WRITE]);
        exit(EXIT_SUCCESS);
    } else {
        // parent
        close(pipe_fd_ptoc[KEY_READ]);
        close(pipe_fd_ctop[KEY_WRITE]);
        uchar value;

        for (int round = 0; round < n; round++) {
            // Write: parent -> child
            if (round == 0) {
                value = 0;
            } else {
                value++;
            }
            write(pipe_fd_ptoc[KEY_WRITE], &value, 1);

            // Read: child -> parent
            while (read(pipe_fd_ctop[KEY_READ], &value, 1) < 0);
            // fprintf(FD_STDOUT, "%d\n", value); // debug code
        }
        close(pipe_fd_ptoc[KEY_WRITE]);
        close(pipe_fd_ctop[KEY_READ]);
        if (wait(0) < 0) {
            fprintf(FD_STDERR, "%s: cannot wait\n", argv[0]);
            exit(EXIT_FAILURE);
        }
    }

    // print # of ticks in nrounds
    printf("# of ticks in %d rounds: %d\n", n, uptime() - start_tick);
    exit(EXIT_SUCCESS);
}
