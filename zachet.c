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
//10. Напишите программу, экспериментально определяющую размер буфера,
// выделяемого операционной системой для организации pipe.

int main(int argc, char *argv[]) {
    int fd[2];
    if (pipe2(fd, O_NONBLOCK) < 0) {
        perror("Can\'t create pipe\n");
        exit(1);
    }
    
    char *str;
    int count = 0;
    str = "a";
    while (write(fd[1], str, 1) == 1) {
        count++;
    }
    printf("%d", count);


    // struct stat buf = {0};
    // fstat(fd[1], &buf);
    // printf("%lld\n", buf.st_size);

    return 0;
}
