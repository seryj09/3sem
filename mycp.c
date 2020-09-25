#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
//#include <getopt.h>


void Copy(const int src, const int dst) {
    char str[4096];
    int n = read(src, str, 4096), p = 0;
    if (n < 0) {
        perror("cp: Permission denied");
        exit(1);
    }
    while (n > 0) {
        p = write(dst, str, n);
        if (p < 0) {
            perror("cp: Permission denied");
            exit(1);
        }
        n -= p;
    }
}


int main(int argc, char *argv[]) {
    int i = 0, counts_files = 0;
    char *source_file, *target_file;
    //считаем количество файлов (не опций) всего (должно быть два файла)
    for (i = 1; i < argc; ++i) {
        if (argv[i][0] != '-') {
            if (counts_files == 0) {
                source_file = argv[i];
                counts_files++;
            }
            else if (counts_files == 1) {
                target_file = argv[i];
                counts_files++;
            }
            else {
                counts_files++;
            }
            
        }
    }
    if (counts_files != 2 ) {
        printf("usage: cp [-R [-H | -L | -P]] [-fi | -n] [-apvXc] source_file target_file\n       cp [-R [-H | -L | -P]] [-fi | -n] [-apvXc] source_file ... target_directory\n");
        return 0;
    }

    int src, dst, ch = 0;
    //открываем src файл только для чтения
    if ((src = open(source_file, O_RDONLY)) < 0) {
        perror("cp: cannot open file");
        exit(1);
    }
    // сделаем флаги для опций -f -v -i
    int e=0,v=0,f=0;
    while ((ch = getopt(argc, argv, "ifv")) != -1) {
        switch (ch) {
            case 'i':
                e=1;
                break;
            case 'v':
                v=1;  
                break;
            case 'f':
                f=1;
                break;
        }
    }
    // сначала удалим dst файл если была опция -f
    if (f == 1) {
        remove(target_file);
    }
    //затем открываем новый dst файл
    if ((dst = open(target_file, O_RDWR | O_CREAT | O_TRUNC, S_IWRITE | S_IREAD)) < 0) {
        perror("cp: file cannot be written");
        exit(1);
    }

    if (e == 1) {
        printf("overwrite %s? (y/n [n])", target_file);
        char *answer;
        scanf("%c", answer);
        if (*answer != 'Y' && *answer != 'y') {
            printf("not overwritten\n");
            return 0;
        }
    }
    if (v == 1) {
        printf("%s->%s\n", source_file, target_file);
    }
    //только сейчас копируем
    Copy(src, dst);
    close(src);
    close(dst);
    return 0;
}
