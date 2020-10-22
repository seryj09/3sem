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

struct msgbuf
{
    long mtype;     /* тип сообщения, должен быть > 0 */
    char mtext[100]; /* содержание сообщения */
};

void Judge(int n, int id)
{
    char str[] = "я пришел", str2[] = "палка";
    struct msgbuf ss, ss2 = {2, "палка"};
    for (int i = 0; i < n; i++)
    {
        if (msgrcv(id, &ss, strlen(str), 1, 0) < 0)
        {
            perror("msgrcv1 judge\n");
            exit(1);
        }
    }
    printf("я судья, все явились\n");

    if (msgsnd(id, &ss2, strlen(str2), 0) < 0) {
        perror("msgsnd judge\n");
        exit(1);
    }
    struct timespec mt1, mt2;

    if (clock_gettime(_CLOCK_REALTIME, &mt1) < 0) {
        perror("time");
        exit(1);
    }

    printf("я судья отправил палку, забег начался\n");
    if (msgrcv(id, &ss, strlen(str), n+2, 0) < 0)
        {
            perror("msgrcv2 judge\n");
            exit(1);
        }

    if (clock_gettime(_CLOCK_REALTIME, &mt2) < 0){
        perror("time");
        exit(1);
    }

    printf("я судья эстафета завершена, время забега %ld ms\n",1000*(mt2.tv_sec - mt1.tv_sec) + (mt2.tv_nsec - mt1.tv_nsec)/1000000 );
}

void Runner(int i, int id)
{
    printf("я бегун под номером %d\n", i);
    char str[] = "я пришел";
    struct msgbuf ss = {1, "я пришел"}, ss2, ssi = {i + 3, "палка"};
    if (msgsnd(id, &ss, strlen(str), 0) < 0)
    {
        perror("msgsnd1 runner\n");
        exit(1);
    }
    if (msgrcv(id, &ss2, strlen(str), i + 2, 0) < 0)
        {
            perror("msgrcv runner\n");
            exit(1);
        }
    if (msgsnd(id, &ssi, strlen(str), 0) < 0)
    {
        perror("msgsnd2 runner\n");
        exit(1);
    }
    printf("я бегун %d передал палку\n", i);
}

int main(int argc, char *argv[])
{
    setbuf(stdout, NULL);
    int n = atoi(argv[1]);
    key_t key = ftok(".", 's');//IPC_PRIVATE
    int id = msgget(key, IPC_CREAT | IPC_EXCL | 0700);
    if (id < 0)
    {
        perror("msgget");
        exit(1);
    }
    else
        printf("created\n");

    for (int i = 0; i < n + 1; i++)
    {
        int pid = fork();

        if (pid == 0)
        {
            if (i == n)
            {
                Judge(n, id);
            }
            else
            {
                Runner(i, id);
            }
            exit(1);
        }
    }

    int status;
    while (wait(&status) != -1){}
    msgctl(id, IPC_RMID, 0);
    return 0;
}