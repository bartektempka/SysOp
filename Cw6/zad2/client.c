#include <stdio.h>
#include <stdlib.h>
#include <mqueue.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <time.h>
#include "shared.h"

mqd_t client_queue;
mqd_t server_queue;
int client_queue_id = -1;
char client_queue_name[20];
char*msg;
int child;

void handle_init(){
    printf("sending init\n");
    fflush(stdout);
    snprintf(msg, MAX_MSG_LEN, "%d%s", INIT, client_queue_name);
    fflush(stdout);
    if (mq_send(server_queue, msg, MAX_MSG_LEN, 0) == -1){
        printf("Error: %s\n", strerror(errno));
    }
    mq_receive(client_queue, msg, MAX_MSG_LEN, NULL);
    memmove(msg, msg+1, strlen(msg));
    client_queue_id = atoi(msg);
    printf("Client id: %d\n", client_queue_id);
    fflush(stdout);
}
void handle_list(){
    printf("sending list\n");
    fflush(stdout);
    snprintf(msg, MAX_MSG_LEN, "%d%d", LIST, client_queue_id);
    mq_send(server_queue, msg, MAX_MSG_LEN, 0);
}

void handle_2all(){
    printf("sending 2all\n");
    fflush(stdout);
    printf("Enter message: ");
    fflush(stdout);
    char buffer[1024];
    scanf("%s", buffer);
    snprintf(msg, MAX_MSG_LEN, "%d%d %s", TALL, client_queue_id, buffer);
    mq_send(server_queue, msg, MAX_MSG_LEN, 0);
}
void handle_2one(){
    printf("sending 2one\n");
    fflush(stdout);
    printf("Enter message: ");
    fflush(stdout);
    char buffer[1024];
    scanf("%s", buffer);
    printf("Enter target id: ");
    int target_id;
    scanf("%d", &target_id);
    snprintf(msg, MAX_MSG_LEN, "%d%d %d %s", TONE, client_queue_id, target_id, buffer);
    mq_send(server_queue, msg, MAX_MSG_LEN, 0);
}
void handle_stop(){
    printf("recived stop\n");
    fflush(stdout);
    exit(0);
}
void handle_exit(){
    printf("exiting");
    fflush(stdout);
    snprintf(msg, MAX_MSG_LEN, "%d%d", STOP, client_queue_id);
    mq_send(server_queue, msg, MAX_MSG_LEN, 0);
    mq_close(client_queue);
    mq_close(server_queue);
    mq_unlink(client_queue_name);
    exit(0);
}
void handler(int signum) {
	handle_exit();
}

void notification_handler(union sigval sv){
    mq_receive(client_queue, msg, MAX_MSG_LEN, NULL);
    printf("\n%s\n",msg);
    printf("Enter command [LIST|2ALL|2ONE|STOP]: ");
    fflush(stdout);
    mq_notify(client_queue, &(struct sigevent){.sigev_notify = SIGEV_THREAD, .sigev_notify_function = notification_handler, .sigev_notify_attributes = NULL});
}
int main(int argc, char** argv){
    struct mq_attr attr = {.mq_curmsgs = mq_def_curmsgs,.mq_flags=mq_def_flags, .mq_maxmsg=mq_def_maxmsg,.mq_msgsize=mq_def_msgsize};
    server_queue = mq_open(SERVER_QUEUE_NAME, O_WRONLY,&attr);
    if(server_queue == -1){
        printf("Error: %s\n", strerror(errno));
        exit(1);
    }
    srand(time(NULL));
    
    sprintf(client_queue_name, "/%d", rand());
    client_queue = mq_open(client_queue_name, O_RDONLY | O_CREAT | O_EXCL, 0666, &attr);
    msg = malloc(sizeof(char)*MAX_MSG_LEN);
    if(client_queue == -1){
        printf("Error: %s\n", strerror(errno));
        exit(1);
    }
    atexit(handle_exit);
    
    char* command = malloc(sizeof(char)*4);
    handle_init();
    mq_notify(client_queue, &(struct sigevent){.sigev_notify = SIGEV_THREAD, .sigev_notify_function = notification_handler});
    signal(SIGINT, handler);
 
    fflush(stdout);
    while(1){
        printf("Enter command [LIST|2ALL|2ONE|STOP]: ");

        scanf("%s", command);

        if(strcmp(command,"LIST") == 0){
            handle_list();
        }
        else if (strcmp("2ALL", command) == 0){
            handle_2all();
        }
        else if (strcmp("2ONE",command)== 0){
            handle_2one();
        }
        else if (strcmp("STOP",command)== 0){
            handle_exit();
        }
        else{
            printf("Wrong command\n");
        }
    }
}