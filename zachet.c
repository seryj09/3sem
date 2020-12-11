#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <getopt.h>
#include <sys/wait.h>
#include <time.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/sem.h>
//10. Напишите программу, экспериментально определяющую размер буфера,
// выделяемого операционной системой для организации pipe.

int main(int argc, char *argv[]) {
    int fd[2];
    if (pipe(fd) < 0) {
        perror("Can\'t create pipe\n");
        exit(1);
    }
    // dup2(fd[1], 1);
    char *str;
    int count = 0;
    str = "a";
    while (write(fd[1], str, 1) > 0) {
        count++;
    }
    write(1, &count, 4);

    // int *pBytes;
    // fseek (&fd[0], 0 , SEEK_END);
    // *pBytes = ftell(&fd[0]);
    // rewind(&fd[0]);
    return 0;
}
