#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

// #include <sys\types.h>
// #include <sys\stat.h>
// #include <io.h>

void Cat(const int fd) {
    char str[4096];
    int n = read(fd, str, 4096), p = 0;
    if (n < 0) {
        perror("open failed");
        exit(1);
    }
    while (n > 0) {
        p = write(1, str, n);
        if (p < 0) {
            perror("open failed");
            exit(1);
        }
        n -= p;
    }
}

int main(int argc, char const *argv[]) {
    if (argc == 1) {
        while (1) 
            Cat(0);
    }

    for (int i = 1; i < argc; i++) {
        int fd = open(argv[i], O_RDWR | O_EXCL);    
        Cat(fd);
    }
    
    return 0;
}