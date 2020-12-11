#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <getopt.h>
#include <sys/wait.h>
#include <time.h>
#include <ctype.h>

int main(int argc, char *argv[]) {
    struct timespec mt1, mt2;

    if (clock_gettime(_CLOCK_REALTIME, &mt1) < 0)
        perror("time");

    int w = 0, ch = 0;
    static struct option longopts[] = {
            { "wc",      no_argument,            NULL,     'w' },
    };
    
    while ((ch = getopt_long(argc, argv, "w", longopts, 0)) != -1) {
        switch (ch) {
            case 'w':
                w=1;
                break;
        }
    }

    
    if (w == 0) {
        pid_t pid = fork();
        if (pid < 0) {
            perror("Can\'t fork child\n");
        }
        if (pid == 0) {

            execvp(argv[2], argv + 2);
            exit(1);
        }
        
        int status;
        while (wait(&status) != -1) {
            usleep(1);
        }
    }
    else {
        int fd[2];
        if (pipe(fd) < 0) {
            perror("Can\'t create pipe\n");
            exit(1);
        }
        pid_t pid = fork();
        if (pid < 0) {
            perror("Can\'t fork child\n");
            exit(1);
        }

        if (pid == 0) {
            dup2(fd[1], 1);
            execvp(argv[3], argv + 3);
        }
        int status;
        while (wait(&status) != -1) {
            usleep(1);
        }   
        int n,i, s=1,w=0;
        char str[4096];
        if ((n = read(fd[0],str, 4096)) < 0) {
            perror("Can\'t read\n");
            exit(1);
        }

        for (i = 0; i < n-1; i++) {
            if (str[i] == '\n') s++;           
            if (isspace(str[i+1]) && isspace(str[i])==0 ) w++;
        }
        
        printf("%8d%8d%8d\n", s, w, n);
    }
    if (clock_gettime(_CLOCK_REALTIME, &mt2) < 0)
         perror("time");

    printf("%ld ms\n", 1000*(mt2.tv_sec - mt1.tv_sec) + (mt2.tv_nsec - mt1.tv_nsec)/1000000 );
    return 0;
}