#include <stdio.h>
#include <stdlib.h>
#include <sys/msg.h> 
#include <sys/ipc.h>
#include <errno.h>
#include <string.h>
#include "shared.h"

key_t server_key;
int client_msgid[MAX_CLIENTS];
int clients_id[MAX_CLIENTS];
int server_msgid;
int last_client_id = 0;


void handle_init(struct msg_buff* msg){
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
    client_msgid[current_client_id] = msgget(msg->data.client_key, 0);
    msg->data.client_queue_id = last_client_id;
    clients_id[current_client_id] = last_client_id;
    last_client_id++;
    msgsnd(client_msgid[current_client_id], msg, sizeof(struct data), 0);
    printf("Client id: %d\n", msg->data.client_queue_id);
    fflush(stdout);

}
void handle_list(struct msg_buff* msg){
    printf("Handling list\n");
    fflush(stdout);
    int current_client_id = 0;
    while (current_client_id < MAX_CLIENTS){
        if (clients_id[current_client_id] == msg->data.client_queue_id){
            break;
        }
        current_client_id++;
    }

    if(current_client_id == MAX_CLIENTS){
        printf("Client not found\n");
        fflush(stdout);
        return;
    }
    strcpy(msg->data.msg, "List of clients: ");
    for (int i = 0; i < MAX_CLIENTS; i++){
        if (clients_id[i] != -1){
            char* client_id = malloc(sizeof(char)*4);
            sprintf(client_id, "%d", clients_id[i]);
            strcat(msg->data.msg, client_id);
            strcat(msg->data.msg, " ");
        }
        
    }
    //strcat(msg->data.msg, "\0");
    printf("%s\n", msg->data.msg);
    msgsnd(client_msgid[current_client_id], msg, sizeof(struct data), 0);
    printf("List sent\n");
    fflush(stdout);
}
void handle_2all(struct msg_buff* msg){
    printf("Handling 2all\n");
    fflush(stdout);
    int current_client_id = 0;
    while (current_client_id < MAX_CLIENTS){
        if (clients_id[current_client_id] == msg->data.client_queue_id){
            break;
        }
        current_client_id++;
    }
    if(current_client_id == MAX_CLIENTS){
        printf("Client not found\n");
        fflush(stdout);
        return;
    }
    char buffer[MAX_MSG_LEN];
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    sprintf(buffer,"now: %d-%02d-%02d %02d:%02d:%02d Sender id: %d\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec, msg->data.client_queue_id);
    strcat(buffer, msg->data.msg);
    strcat(buffer, "\0");
    strcpy(msg->data.msg, buffer);

    for (int i = 0; i < MAX_CLIENTS; i++){
        if (clients_id[i] != -1 && i != current_client_id){ //dont self send
            msgsnd(client_msgid[i], msg, sizeof(struct data), 0);
        }
    }
}
void handle_2one(struct msg_buff* msg){
    printf("Handling 2one\n");
    fflush(stdout);
    int current_client_id = 0;
    while (current_client_id < MAX_CLIENTS){
        if (clients_id[current_client_id] == msg->data.client_queue_id){
            break;
        }
        current_client_id++;
    }
    if(current_client_id == MAX_CLIENTS){
        printf("Client not found\n");
        fflush(stdout);
        return;
    }
    int receiver_client_id = 0;
    while (receiver_client_id < MAX_CLIENTS){
        if (clients_id[receiver_client_id] == msg->data.client_id){
            break;
        }
        receiver_client_id++;
    }
    if(receiver_client_id == MAX_CLIENTS){
        printf("Receiver not found\n");
        fflush(stdout);
        return;
    }
    printf("Receiver id: %d\n", msg->data.client_id);
    fflush(stdout);
    char buffer[MAX_MSG_LEN];
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    sprintf(buffer,"now: %d-%02d-%02d %02d:%02d:%02d Sender id: %d\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec, msg->data.client_queue_id);
    strcat(buffer, msg->data.msg);
    strcat(buffer, "\0");
    strcpy(msg->data.msg, buffer);
    printf("%s\n", msg->data.msg);
    fflush(stdout);
    if(-1 == msgsnd(client_msgid[receiver_client_id], msg, sizeof(struct data), 0)){
        printf("Error sending message\n");
        fflush(stdout);
    }
    printf("Message sent\n");
    fflush(stdout);
}
void handle_stop(struct msg_buff * msg){
    printf("Handling stop\n");
    fflush(stdout);
    int current_client_id = 0;
    while (current_client_id < MAX_CLIENTS){
        if (clients_id[current_client_id] == msg->data.client_queue_id){
            break;
        }
        current_client_id++;
    }
    if(current_client_id == MAX_CLIENTS){
        printf("Client not found\n");
        fflush(stdout);
        return;
    }
    // msgctl(client_msgid[current_client_id], IPC_RMID, NULL);
    clients_id[current_client_id] = -1;
    client_msgid[current_client_id] = -1;
    printf("Client %d stopped\n", msg->data.client_queue_id);
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
    msgctl(server_msgid, IPC_RMID, NULL);
}
int main(int argc, char** argv){
    printf("Server started\n");
    fflush(stdout);
    atexit(handle_exit);
    memset(clients_id, -1, sizeof(clients_id));
    memset(client_msgid, -1, sizeof(client_msgid));
    server_key = ftok(getenv("HOME"), PROJECT_ID);
    if(server_key == -1)
        printf("Error: %s", strerror(errno));
    printf("server key: %d\n", server_key);
    if ((server_msgid = msgget(server_key, 0666 | IPC_CREAT)) == -1) {
        perror ("msgget");
        exit (1);
    }
    struct msg_buff *msg = malloc(sizeof(struct msg_buff));
    fflush(stdout);
    while (1){
        msgrcv(server_msgid, msg, sizeof(struct data), -6, 0);
        switch (msg->msg_type)
        {
        case INIT:
            handle_init(msg);
            break;
        case LIST:
            handle_list(msg);
            break; 
        case TALL:
            handle_2all(msg);
            break;
        case TONE:
            handle_2one(msg);
            break;
        case STOP:
            handle_stop(msg);
            break;
        default:
            break;
        }
        
    }
}
