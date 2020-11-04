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
#include <sys/shm.h>

union semun {
               int              val;    /* Value for SETVAL */
               struct semid_ds *buf;    /* Buffer for IPC_STAT, IPC_SET */
               unsigned short  *array;  /* Array for GETALL, SETALL */
               struct seminfo  *__buf;  /* Buffer for IPC_INFO
                                           (Linux-specific) */
};


int Wait(int id, int semnum) {
    struct sembuf sops = {semnum, 0, 0};
    return semop(id, &sops, 1);
}
int Enter(int id, int semnum) {
    struct sembuf sops = {semnum, -1, 0};
    return semop(id, &sops, 1);

}

int Exit(int id, int semnum) {
    struct sembuf sops = {semnum, 1, 0};
    return semop(id, &sops, 1);

}


int main(int argc, char const *argv[]) {
    setbuf(stdout, NULL);
    key_t key1 = ftok(".", 1);
    key_t key2 = ftok("..", 2);
    int semid, shmid;
    if ((semid = semget(key1, 2, IPC_CREAT | IPC_EXCL | 0666)) < 0) {
        perror("semget");
        exit(1);
    }
    if ((shmid = shmget(key2, 10000, IPC_CREAT | IPC_EXCL | 0666)) < 0) {
        perror("shmget");
        exit(1);
    }

    union semun arg1, arg2;
    arg1.val = 1;
    arg2.val = 1;
    if (semctl(semid, 0, SETVAL, arg1) == -1) {
        perror("semctl");
        exit(1);
    }
    if (semctl(semid, 0, SETVAL, arg2) == -1) {
        perror("semctl");
        exit(1);
    }
    
    char *addr, *s;
    addr = (char*) shmat(shmid, NULL, 0);
    while (1) {
        Wait(semid, 0);
        if (addr == (char*) -1) {
            perror("addr");
            exit(1);
        }
        printf("%s", addr);
        Enter(semid, 1);
	Exit(semid, 0);
    }
    return 0;
}
