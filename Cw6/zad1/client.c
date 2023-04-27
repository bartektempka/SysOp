#include <stdio.h>
#include <stdlib.h>
#include <sys/msg.h> 
#include <sys/ipc.h>
#include <errno.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include "shared.h"
key_t client_key;
key_t server_key;
int client_id;
int client_msgid;
int server_msgid;
struct msg_buff *msg;
int child;

void handle_init(){
    printf("sending init\n");
    fflush(stdout);
    msg->msg_type = INIT;
    msg->data.client_key = client_key;
    int testSend = msgsnd(server_msgid, msg, sizeof(struct data), 0);
    int testRecive = msgrcv(client_msgid, msg, sizeof(struct data), 0, 0);
    if (testSend == -1 || testRecive == -1){
        printf("Error: %s", strerror(errno));
        fflush(stdout);
    }
    
    client_id = msg->data.client_queue_id;
    
}
void handle_list(){
    printf("sending list\n");
    fflush(stdout);
    msg->msg_type = LIST;
    msg->data.client_queue_id = client_id;
    msgsnd(server_msgid, msg, sizeof(struct data), 0);
}
void handle_2all(){
    printf("sending 2all\n");
    fflush(stdout);
    msg->msg_type = TALL;
    msg->data.client_queue_id = client_id;
    printf("Enter message: ");
    fflush(stdout);
    scanf("%s", msg->data.msg);
    msgsnd(server_msgid, msg, sizeof(struct data), 0);
}
void handle_2one(){
    printf("sending 2one\n");
    fflush(stdout);
    msg->msg_type = TONE;
    msg->data.client_queue_id = client_id;
    printf("Enter message: ");
    fflush(stdout);
    scanf("%s", msg->data.msg);
    printf("Enter target id: ");
    scanf("%d", &(msg->data.client_id));
    msgsnd(server_msgid, msg, sizeof(struct data), 0);

}
void handle_stop(){
    printf("sending stop\n");
    fflush(stdout);
    kill(child, SIGKILL);
    msg->msg_type = STOP;
    msg->data.client_queue_id = client_id;
    msgsnd(server_msgid, msg, sizeof(struct data), 0);
    exit(0);

}
void handler(int signum) {
	handle_stop();
}
void exit_handler(){
    msgctl(client_msgid, IPC_RMID, NULL);

}
int main(int argc, char** argv){
    msg = malloc(sizeof(struct msg_buff));
    srand(time(NULL));
    client_key = ftok(getenv("HOME"), rand()%255 +1);
    printf("Client key: %d\n", client_key);
    fflush(stdout);
    client_msgid = msgget(client_key, 0666 | IPC_CREAT);
    server_key = ftok(getenv("HOME"), PROJECT_ID);
    server_msgid = msgget(server_key, 0);
    
    atexit(exit_handler);
    
    char* command = malloc(sizeof(char)*4);
    handle_init();
    printf("Client id: %d\n", client_id);
    fflush(stdout);
    child = fork();
    if(child == 0){
        while(1){
            msgrcv(client_msgid, msg, sizeof(struct data), 0, 0);
            if(msg->msg_type == TONE || msg->msg_type == TALL || msg->msg_type == LIST){
                printf("%s\n", msg->data.msg);
                printf("Eneter command: ");
            }
            fflush(stdout);
        }
    }
    signal(SIGINT, handler);
    printf("Enter command: ");
    fflush(stdout);
    while(1){
        

        scanf("%s", command);

        if(strcmp(command,"LIST") == 0){
            handle_list();
        }
        else if (strcmp("2ALL", command) == 0){
            handle_2all();
            printf("Enter command: ");
            fflush(stdout);
        }
        else if (strcmp("2ONE",command)== 0){
            handle_2one();
            printf("Enter command: ");
            fflush(stdout);
        }
        else if (strcmp("STOP",command)== 0){
            handle_stop();
        }
        else{
            printf("Wrong command\n");
            printf("Enter command: ");
            fflush(stdout);
        }
    }
}