#include <stdio.h>
#include <string.h>
int main(int argc, char const *argv[]) {
    if (argc == 1) {
        printf("\n");
        return 0;
    } 
    int k = 1;
    if (strcmp(argv[1], "-n") == 0){
        ++k;
    }
    for (int i = k; i < argc-1; i++) {
        printf("%s ", argv[i]);
    }
    printf("%s", argv[argc - 1]);
    if (k == 1){
        printf("\n");
    }
    return 0;
}