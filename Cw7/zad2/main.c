#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#include <sys/mman.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <semaphore.h>
enum vars{
    M = 15,
    N = 10,
    P = 5,
    F = 5
};
char* sem_name_chair = "/chair";
char* sem_name_wait = "/waiting";
char* sem_name_memory = "/memory";

void handler(int signum){
    sem_unlink("/chair");
    sem_unlink("/waiting");
    sem_unlink("/memory");
    shm_unlink("/client_times");
    exit(0);
}

void haidresser(){
    sem_t *wait_sem = sem_open(sem_name_wait,0);
    sem_t *chair_sem = sem_open(sem_name_chair,0);
    sem_t *memory_sem = sem_open(sem_name_memory,0);
    if(wait_sem == SEM_FAILED || chair_sem == SEM_FAILED || memory_sem == SEM_FAILED){
        printf("Error sem: %s\n",strerror(errno));
    }
    int shm = shm_open("/client_times",O_RDWR,0666);
    if(shm == -1){
        printf("Error shm: %s\n",strerror(errno));
    }
    int* client_times;
    printf("Haidresser is waiting\n");
    // int sem_val;
    // if(-1 == sem_getvalue(chair_sem,&sem_val)){
    //     printf("Error sem: %s\n",strerror(errno));
    // }
    // printf("chair_sem: %d\n",sem_val);
    while(1){
        sem_wait(chair_sem);
        printf("Haidresser took chair\n");
        sem_wait(wait_sem);
        printf("waiting form mem\n");
        sem_wait(memory_sem);
        int* client_times = mmap(NULL,sizeof(int)*(P+1),PROT_READ | PROT_WRITE,MAP_SHARED,shm,0);
        printf("Haidresser is taking client %d,%d\n",client_times[0]+1,client_times[client_times[0]+1]);
        int f = client_times[client_times[0]+1];
        client_times[client_times[0]+1] = -1;
        client_times[0] = (client_times[0] + 1) % P;
        munmap(client_times,sizeof(int)*(P+1));
        sem_post(memory_sem);
        printf("Haidresser is cutting hair %d\n",f);
        sleep(f);
        printf("Haidresser is done\n");
        sem_post(chair_sem);
    }  
}
int main(int argc,char** argv){
    
    int const Fs[F] = {2,4,6,8,10};
    setbuf(stdout, NULL);
    srand(time(NULL));
    sem_t *wait_sem = sem_open(sem_name_wait,O_CREAT,0666,0);
    sem_t *chair_sem = sem_open(sem_name_chair,O_CREAT,0666,0);
    for(int i = 0;i < N; i++){
        sem_post(chair_sem);
        }
    sem_t *memory_sem = sem_open(sem_name_memory,O_CREAT,0666,0);
    sem_post(memory_sem); //idk why initial semaphore value doesnt work
    // int sem_val;
    // if(-1 == sem_getvalue(chair_sem,&sem_val)){
    //     printf("Error sem: %s\n",strerror(errno));
    //     return -1;
    // }
    // printf("first sem_chair: %d\n",sem_val);
    sem_close(chair_sem);
    if(wait_sem == SEM_FAILED || chair_sem == SEM_FAILED || memory_sem == SEM_FAILED){
        printf("Error: %s\n",strerror(errno));
        return 1;
    }
    int shm = shm_open("/client_times",O_CREAT | O_RDWR,0666);
    if(shm == -1){
        printf("Error: %s\n",strerror(errno));
        return 1;
    }
    ftruncate(shm,sizeof(int)*(P+1));
    int* client_times = (int*) mmap(NULL,sizeof(int)*(P+1),PROT_READ | PROT_WRITE,MAP_SHARED,shm,0);
    memset(client_times,-1,(1+P)*sizeof(int));
    client_times[0] = 0;
    munmap(client_times,sizeof(int)*(P+1));


    for(int i = 0;i<M;i++){
        if(fork() == 0){
            haidresser();
            return 0;
        }
    }


    int last_client_waiting = 0;
    

    while(1){
        sleep(rand() % (1 + 1));
        printf("New client!\n");
        int f = Fs[ rand() % F];
        printf("Client wants haircut that takes %ds\n",f);
        sem_wait(memory_sem);
        client_times = (int*) mmap(NULL,sizeof(int)*(P+1),PROT_READ | PROT_WRITE,MAP_SHARED,shm,0);
        if(client_times[last_client_waiting+1] == -1){
            client_times[last_client_waiting+1] = f;
            munmap(client_times,sizeof(int)*(P+1));
            sem_post(wait_sem);
            sem_post(memory_sem);
            last_client_waiting = (last_client_waiting + 1) % P;
            printf("Client is waiting for service\n");
            
        }
        else{
            printf("No room, Client is leaving\n");
            munmap(client_times,sizeof(int)*(P+1));
            sem_post(memory_sem);
        }
    }
    while(1){
        sleep(1);}

}