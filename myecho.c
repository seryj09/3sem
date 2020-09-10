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
    for (int i = k; i < argc; i++) {
        printf("%s", argv[i]);
        if (i != argc - 1) {
            printf(" ");
        }
    }
    if (k == 1){
        printf("\n");
    }
    return 0;
}