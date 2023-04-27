#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <signal.h>

#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/sem.h>

enum vars{
    M = 15,
    N = 10,
    P = 5,
    F = 5
};

key_t key;
struct sembuf  waiting_sem_decrese= {.sem_num = 0,.sem_op = -1,.sem_flg = 0};
struct sembuf waiting_sem_increse={.sem_num = 0,.sem_op = 1,.sem_flg = 0};
struct sembuf chair_sem_take = {.sem_num = 1,.sem_op = -1,.sem_flg = 0};
struct sembuf chair_sem_free = {.sem_num = 1,.sem_op = 1,.sem_flg = 0};
struct sembuf shmem_sem_increse={.sem_num = 2,.sem_op = 1,.sem_flg = 0};
struct sembuf shmem_sem_decrese={.sem_num = 2,.sem_op = -1,.sem_flg = 0};

void handler(int signum){
    semctl(semget(key,3,0),0,IPC_RMID);
    shmctl(shmget(key,sizeof(int)*(P+1),0),IPC_RMID,NULL);
    exit(0);
}
void haidresser(){
    int sem = semget(key,3,0);
    int shm = shmget(key,sizeof(int)*(P+1),0);
    int* client_times;
    signal(SIGINT,handler);
    printf("Haidresser is waiting\n");
    while(1){
        semop(sem,&chair_sem_take,1);
        printf("Haidresser took chair\n");
        semop(sem,&waiting_sem_decrese,1);
        semop(sem,&shmem_sem_decrese,1);
        client_times = shmat(shm,NULL,0);
        printf("Haidresser is taking client %d,%d\n",client_times[0]+1,client_times[client_times[0]+1]);
        int f = client_times[client_times[0]+1];
        client_times[client_times[0]+1] = -1;
        client_times[0] = (client_times[0] + 1) % P;
        shmdt(client_times);
        semop(sem,&shmem_sem_increse,1);
        printf("Haidresser is cutting hair %d\n",f);
        sleep(f);
        printf("Haidresser is done\n");
        semop(sem,&chair_sem_free,1);
    }  
}
int main(int argc,char** argv){
    setbuf(stdout, NULL);
    int const Fs[F] = {10,10,10,10,10};
    srand(time(NULL));
    key = ftok("main.c", rand()%255 +1);
    int sem = semget(key,3,IPC_CREAT | 0666);
    int shm = shmget(key,sizeof(int)*(P+1),IPC_CREAT | 0666);
    if (shm == -1){
        printf("Error shm: %s\n",strerror(errno));
    }
    int* client_times = (int*) shmat(shm,NULL,0);
    memset(client_times,-1,(1+P)*sizeof(int));
    client_times[0] = 0;
    shmdt(client_times);
    semctl(sem,0,SETVAL,0);
    semctl(sem,1,SETVAL,N);
    semctl(sem,2,SETVAL,1);


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
        semop(sem,&shmem_sem_decrese,1);
        client_times = (int*) shmat(shm,NULL,0);
        if(client_times[last_client_waiting+1] == -1){
            client_times[last_client_waiting+1] = f;
            shmdt(client_times);
            semop(sem,&shmem_sem_increse,1);
            last_client_waiting = (last_client_waiting + 1) % P;
            printf("Client is waiting for service\n");
            if(-1 == semop(sem,&waiting_sem_increse,1)){
                printf("Error: %s\n",strerror(errno));
            }
        }
        else{
            printf("No room, Client is leaving\n");
            shmdt(client_times);
            semop(sem,&shmem_sem_increse,1);
        }
    }
    while(1){
        sleep(1);}

}