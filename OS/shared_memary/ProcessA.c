#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <wait.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/types.h>


int main(int argc, char *argv[])
{
    int message = 0;
    const int SIZE = 4096;
    const char *name = "OS";
    int shm_fd;
    void *flagptr, *curr;
    shm_fd = shm_open(name, O_CREAT | O_RDWR, 0666);
    ftruncate(shm_fd, SIZE);

    curr = flagptr = mmap(0, SIZE, PROT_WRITE, MAP_SHARED, shm_fd, 0);
    sprintf(flagptr, "%d", 0);


    pid_t pid;
    pid = fork();
    if (pid < 0) {              //error
        fprintf(stderr,"Fork Failed");
    }
    else if (pid == 0) {        //chlid
        execlp("./ProcessB","ProcessB",NULL);
    }
    else {                      //parent
        curr+=20;
        while(1)
        {
            printf("1. 輸入正整數回傳二進位\n");
            printf("2. 輸入-1回傳目前最多1的\n");
            printf("3. 輸入-2回傳目前最少1的\n");
            printf("請輸入一個整數 : ");
            scanf("%d",&message);
            sprintf(curr, "%d", message);
            sprintf(flagptr, "%d", 1);
            usleep(100000);

            printf(">%s\n\n",(char *)curr);
        }
    }


    return 0;
}
