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

//передавать по строчно данные от клиента к серверу. Сервер выводит их на экран. Сервер не знает количествот клиентов

int Enter(int id, int semnum) {
    struct sembuf sops = {semnum, -1, 0};
    return semop(id, &sops, 1);
}

int Exit(int id, int semnum) {
    struct sembuf sops = {semnum, 1, 0};
    return semop(id, &sops, 1);
}

int Wait(int id, int semnum) {
    struct sembuf sops = {semnum, 0, 0};
    return semop(id, &sops, 1);
}

void client_c(const int fd, int shmid, int semid)
{
    FILE *stream = fdopen(fd, "r");
    char str[60], *addr, *s;
    addr = (char *)shmat(shmid, NULL, 0);
    if (addr == (char*) -1) {
            perror("semctl");
            exit(1);
        }
    
    while(!feof(stream)) {
    	if (fgets(addr, 60, stream)) {
        	Enter(semid, 0);
        	Wait(semid, 1);
	        //Exit(semid, 0);
		Exit(semid, 1);
    	}
    }
    fclose(stream);
    if (shmdt(addr) < 0) {
         perror("shmdt");
         exit(1);
    }
}

int main(int argc, char const *argv[])
{
    key_t key1 = ftok(".", 1);
    key_t key2 = ftok("..", 2);
    int semid, shmid;
    if ((semid = semget(key1, 2, 0666)) < 0)
    {
        perror("semget");
        exit(1);
    }
    if ((shmid = shmget(key2, 0, 0666)) < 0)
    {
        perror("shmget");
        exit(1);
    }

    
    if (argc == 1)
    {
        client_c(0, shmid, semid);
        // return 0;
    }
    else
    {
        for (int i = 1; i < argc; i++)
        {
            int fd;
            if ((fd = open(argv[i], O_RDWR | O_EXCL)) < 0)
            {
                perror("open failed");
                exit(1);
            }
            client_c(fd, shmid, semid);
            close(fd);
        }
    }

    //semctl(semid, IPC_RMID, 0);
    //shmctl(shmid, IPC_RMID, 0);
    return 0;
}
