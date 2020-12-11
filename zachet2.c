#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/poll.h>

int main(int argc, char *argv[]) {
    int fd[2];
    if (pipe(fd) < 0) {
        perror("Can't create pipe\n");
        exit(1);
    }


    struct pollfd fd_struct = {
        .fd = fd[1],
        .events = POLLOUT
    };

    char byte = 'a';
    off_t size = 0;
    while (poll(&fd_struct, 1, 100) && fd_struct.revents & POLLOUT) {
        write(fd[1], &byte, 1);
        size++;
    }

    printf("Size of buffer = %ld\n", size);

    return 0;
}
