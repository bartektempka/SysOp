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

mqd_t server_queue;
mqd_t client_queue[MAX_CLIENTS];
int clients_id[MAX_CLIENTS];
int last_client_id = 0;
char msg[MAX_MSG_LEN];


void handle_init(){
    printf("Handling init\n");
    fflush(stdout);

    int current_client_id = 0;
    while (current_client_id < MAX_CLIENTS){
        if (clients_id[current_client_id] == -1){
            break;
        }
        current_client_id++;
    }
    if (current_client_id == MAX_CLIENTS){
        printf("Too many clients\n");
        fflush(stdout);
        return;
    }
    memmove(msg, msg+1, strlen(msg));
    client_queue[current_client_id] = mq_open(msg, O_WRONLY);
    clients_id[current_client_id] = last_client_id;
    last_client_id++;

    snprintf(msg, MAX_MSG_LEN, "%d%d", INIT,clients_id[current_client_id]);
    mq_send(client_queue[current_client_id], msg, MAX_MSG_LEN, 0);
    printf("Client id: %d\n", clients_id[current_client_id]);
    fflush(stdout);

}
void handle_list(){
    printf("Handling list\n");
    fflush(stdout);
    memmove(msg, msg+1, strlen(msg));
    int client_queue_id = atoi(msg);
    int client_index = 0;
    while (client_index < MAX_CLIENTS){
        if (clients_id[client_index] == client_queue_id){
            break;
        }
        client_index++;
    }

    if(client_index == MAX_CLIENTS){
        printf("Client not found\n");
        fflush(stdout);
        return;
    }
    strcpy(msg, "List of clients: ");
    for (int i = 0; i < MAX_CLIENTS; i++){
        if (clients_id[i] != -1){
            char* client_id = malloc(sizeof(char)*4);
            sprintf(client_id, "%d", clients_id[i]);
            strcat(msg, client_id);
            strcat(msg, " ");
        }
        
    }
    strcat(msg, "\0");
    printf("%s\n", msg);
    mq_send(client_queue[client_index], msg, MAX_MSG_LEN, 0);
    printf("List sent\n");
    fflush(stdout);
}
void handle_2all(){
    printf("Handling 2all\n");
    fflush(stdout);
    memmove(msg, msg+1, strlen(msg));
    char * token;
    token = strtok(msg, " ");
    int client_queue_id = atoi(token);
    token = strtok(NULL, "");
    int client_index = 0;
    while (client_index < MAX_CLIENTS){
        if (clients_id[client_index] == client_queue_id){
            break;
        }
        client_index++;
    }

    if(client_index == MAX_CLIENTS){
        printf("Client not found\n");
        fflush(stdout);
        return;
    }

    char buffer[MAX_MSG_LEN];
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    sprintf(buffer,"now: %d-%02d-%02d %02d:%02d:%02d Sender id: %d\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec, client_queue_id);
    strcat(buffer, token);
    strcat(buffer, "\0");
    strcpy(msg, buffer);

    for (int i = 0; i < MAX_CLIENTS; i++){
        if (clients_id[i] != -1 && i != client_index){ //dont self send
                mq_send(client_queue[i], msg, MAX_MSG_LEN, 0);
        }
    }
}
void handle_2one(){
    printf("Handling 2one\n");
    fflush(stdout);
    memmove(msg, msg+1, strlen(msg));
    char * token;
    token = strtok(msg, " ");
    int client_queue_id = atoi(token);
    
    int client_index = 0;
    while (client_index < MAX_CLIENTS){
        if (clients_id[client_index] == client_queue_id){
            break;
        }
        client_index++;
    }

    if(client_index == MAX_CLIENTS){
        printf("Client not found\n");
        fflush(stdout);
        return;
    }
    token = strtok(NULL, " ");
    int receiver_client_id = atoi(token);
    int reciver_index = 0;
    while (receiver_client_id < MAX_CLIENTS){
        if (clients_id[reciver_index] == receiver_client_id){
            break;
        }
        reciver_index++;
    }
    if(reciver_index == MAX_CLIENTS){
        printf("Receiver not found\n");
        fflush(stdout);
        return;
    }
    token = strtok(NULL,"");
    printf("Receiver id: %d\n", receiver_client_id);
    fflush(stdout);
    char buffer[MAX_MSG_LEN];
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    sprintf(buffer,"now: %d-%02d-%02d %02d:%02d:%02d Sender id: %d\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec, client_queue_id);
    strcat(buffer, token);
    strcat(buffer, "\0");
    strcpy(msg, buffer);
    printf("%s\n", msg);
    fflush(stdout);
    mq_send(client_queue[reciver_index], msg, MAX_MSG_LEN, 0);
    printf("Message sent\n");
    fflush(stdout);
}
void handle_stop(){
    printf("Handling stop\n");
    fflush(stdout);
    memmove(msg, msg+1, strlen(msg));
    int client_queue_id = atoi(msg);
    
    int client_index = 0;
    while (client_index < MAX_CLIENTS){
        if (clients_id[client_index] == client_queue_id){
            break;
        }
        client_index++;
    }

    if(client_index == MAX_CLIENTS){
        printf("Client not found\n");
        fflush(stdout);
        return;
    }
    mq_close(client_queue[client_index]);
    clients_id[client_index] = -1;
    client_queue[client_index] = -1;
    printf("Client %d stopped\n", client_queue_id);
    fflush(stdout);
    for (int i = 0; i < MAX_CLIENTS; i++){
        if (clients_id[i] != -1){
            return;}
    }
    printf("No clients left, server stopped\n");
    fflush(stdout);

    exit(0);
}
void handle_exit(){
    for (int i = 0; i < MAX_CLIENTS; i++){
        if (clients_id[i] != -1){
            mq_send(client_queue[i],"5",MAX_MSG_LEN,5);
            mq_close(client_queue[i]);
            }
    }
    mq_close(server_queue);
    mq_unlink(SERVER_QUEUE_NAME);
}
void handler(int signum) {
	handle_exit();
}

int main(int argc, char** argv){
    printf("Server started\n");
    fflush(stdout);
    atexit(handle_exit);
    signal(SIGINT, handler);
    memset(clients_id, -1, sizeof(clients_id));
    memset(client_queue, -1, sizeof(client_queue));
    struct mq_attr attr = {.mq_curmsgs = mq_def_curmsgs,.mq_flags=mq_def_flags, .mq_maxmsg=mq_def_maxmsg,.mq_msgsize=mq_def_msgsize};

    server_queue = mq_open(SERVER_QUEUE_NAME, O_CREAT | O_RDWR, 0666, &attr);
    if (server_queue == -1){
        printf("Error creating server queue\n");
        fflush(stdout);
        exit(1);
    }
    
    while (1){
        printf("Waiting for mess\n");
        fflush(stdout);
        if (-1 == mq_receive(server_queue, msg, MAX_MSG_LEN, NULL)){
            printf("Error: %s\n", strerror(errno));
            exit(0);
        }else{
            printf("recive sucesfull\n");
        }
        fflush(stdout);
        for (int i = 0;i<MAX_MSG_LEN;i++){
            printf("%c",msg[i]);
            fflush(stdout);
        }
        printf("\n");
        printf("%s\n",msg);
        fflush(stdout);
        switch (msg[0] - '0')
        {
        case INIT:
            handle_init();
            break;
        case LIST:
            handle_list();
            break; 
        case TALL:
            handle_2all();
            break;
        case TONE:
            handle_2one();
            break;
        case STOP:
            handle_stop();
            break;
        default:
            break;
        }
        
    }
}
