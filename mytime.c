#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <getopt.h>
#include <sys/wait.h>
#include <time.h>

int main(int argc, char *argv[]) {
    struct timespec mt1, mt2;

    if (clock_gettime(_CLOCK_REALTIME, &mt1) < 0)
        perror("time");

    pid_t pid = fork();
    if (pid == 0) {
        execvp(argv[1], argv + 1);
        exit(1);
    }
    
    int status;
    while (wait(&status) != -1) {
        usleep(1);
    }

    if (clock_gettime(_CLOCK_REALTIME, &mt2) < 0)
         perror("time");

    printf("%ld,%03ld sec", mt2.tv_sec - mt1.tv_sec, (mt2.tv_nsec - mt1.tv_nsec)/1000000 );
    return 0;
}