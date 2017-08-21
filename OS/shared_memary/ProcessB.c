#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>


typedef struct data {
    char bin[100];
    int num_1;
}data;


int main(int argc, char *argv[])
{
    data shm[1000];
    int count = 0;
    char binary[100] = "";
    char tt[10000] = "";
    const int SIZE = 4096;
    const char *name = "OS";
    int shm_fd;
    void *flagptr, *curr;
    int flag,message,tmp;
    shm_fd = shm_open(name, O_RDWR,0666);
    curr = flagptr = mmap(0, SIZE, PROT_WRITE, MAP_SHARED, shm_fd, 0);
    curr += 20;

    while(1)
    {
        sscanf((char *)flagptr,"%d",&flag);
        if(flag==1)
        {
            sprintf(flagptr,"%d",0);
            sscanf((char *)curr,"%d",&message);
            switch (message) {
                case -1 :
                    if(count == 0)
                    {
                        sprintf(curr,"%s","please enter data");
                        continue;   
                    }
                    tmp = shm[0].num_1;
                    for (int i = 1; i < count; ++i) {
                        if(shm[i].num_1 > tmp)
                            tmp=shm[i].num_1;
                    }
                    sprintf(tt,"%s","");
                    for (int i = 0; i < count; ++i) {
                        if(shm[i].num_1==tmp)
                            sprintf(tt, "%s %s",tt,shm[i].bin);
                    }

                    sprintf(curr,"%s",tt);
                    break;
                case -2 :
                    if(count == 0)
                    {
                        sprintf(curr,"%s","please enter data");
                        continue;   
                    }
                    tmp = shm[0].num_1;
                    for (int i = 1; i < count; ++i) {
                        if(shm[i].num_1 < tmp)
                            tmp=shm[i].num_1;
                    }
                    sprintf(tt,"%s","");
                    for (int i = 0; i < count; ++i) {
                        if(shm[i].num_1==tmp)
                            sprintf(tt, "%s %s",tt,shm[i].bin);
                    }

                    sprintf(curr,"%s",tt);
                    break;
                default:
                    sprintf(tt,"%d:",message);
                    tmp=0;
                    shm[count].num_1 = 0;
                    do{
                        if (message %2 == 1) 
                            shm[count].num_1++;
                            
                        binary[tmp++] = message % 2 + '0';
                    }while(message /= 2);
                    for(int i=tmp-1;i>=0;i--)
                        sprintf(tt,"%s%c",tt,binary[i]);

                    sprintf(shm[count].bin,"%s;",tt);
                    sprintf(curr,"%s",shm[count].bin);
                    count++;
            }

        }
    }

    shm_unlink(name);

    return 0;
}
