#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>
#include<sys/un.h>
#include<pthread.h>
#include<signal.h>
#include "shared.h"
char buffer[MAX_MSG_LEN] = { 0 };
int server_fd;
void connect_inet(int port, char* ip, char* client_name){
    struct sockaddr_in serv_addr;
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        exit(EXIT_FAILURE);
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
    if (inet_pton(AF_INET, ip, &serv_addr.sin_addr)
        <= 0) {
        printf(
            "\nInvalid address/ Address not supported \n");
        exit(EXIT_FAILURE);
    }
    if ((connect(server_fd, (struct sockaddr*)&serv_addr,
                   sizeof(serv_addr)))
        < 0) {
        printf("\nConnection Failed \n");
        exit(EXIT_FAILURE);
    }

    send(server_fd, client_name, strlen(client_name), 0);
}

void connect_unix(char* path, char* client_name){
    struct sockaddr_un serv_addr;
    if ((server_fd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        exit(EXIT_FAILURE);
    }
    serv_addr.sun_family = AF_UNIX;
    strcpy(serv_addr.sun_path, path);
    if ((connect(server_fd, (struct sockaddr*)&serv_addr,
                   sizeof(serv_addr)))
        < 0) {
        printf("\nConnection Failed \n");
        exit(EXIT_FAILURE);
    }
    
    send(server_fd, client_name, strlen(client_name), 0); 
}
void handle_list(){
    send(server_fd, "2", 1, 0);
    //read(server_fd, buffer, MAX_MSG_LEN);
    //printf("%s\n", buffer);
}
void handle_2all(char* msg){
    strcpy(buffer, "3");
    strcat(buffer, msg);
    printf("%s\n", buffer);
    send(server_fd, buffer, strlen(buffer), 0);
}
void handle_2one(char* name,char* msg){
    strcpy(buffer, "4");
    strcat(buffer, name);
    strcat(buffer, " ");
    strcat(buffer, msg);
    send(server_fd, buffer, strlen(buffer), 0);
}
void handle_exit(){
    send(server_fd, "5", 1, 0);
    shutdown(server_fd, SHUT_RDWR);
    close(server_fd);
    exit(EXIT_SUCCESS);
}
void *recive_handle(void* arg){
    while(1){
        read(server_fd, buffer, MAX_MSG_LEN);
        if(strcmp(buffer, "PING") == 0){
            send(server_fd, "6", 1, 0);
        }
        else if(strcmp(buffer, "STOP") == 0){
            handle_exit();
        }
        else{
            printf("%s\n", buffer);
            printf("Enter command [LIST|2ALL string|2ONE client_name string|STOP]: \n");
            fflush(stdout);
        }
    }
}
void sigint_handler(int signo){
    handle_exit();
}
int main(int argc, char const* argv[])
{   
    char client_name[MAX_NAME_LEN];
    strcpy(client_name, argv[1]);
    if(strcmp(argv[2],"inet") == 0){
        int port = atoi(argv[3]);
        char ip[104];
        strcpy(ip, argv[4]);
        connect_inet(port, ip, client_name);
    }
    else if(strcmp(argv[2],"unix") == 0){
        char path[104];
        strcpy(path, argv[3]);
        connect_unix(path, client_name);
    }
    else{
        printf("Wrong arguments\n");
        exit(EXIT_FAILURE);
    }
    printf("Connected\n");

    read(server_fd,buffer,MAX_MSG_LEN); //ping
    printf("Server: %s\n", buffer);

    signal(SIGINT, sigint_handler);
    char command[MAX_MSG_LEN];
    char arg1[MAX_MSG_LEN];
    char arg2[MAX_MSG_LEN];

    pthread_t recive_thread;
    pthread_create(&recive_thread, NULL, recive_handle, NULL);
    
    while(1){
        
        printf("Enter command [LIST|2ALL string|2ONE client_name string|STOP]: \n");
        scanf("%s", command); 
        printf("%s\n", command);
        if(strcmp(command,"LIST") == 0){
            handle_list();
        }
        else if (strcmp("2ALL", command) == 0){
            scanf("%s", arg1);
            handle_2all(arg1);
        
        }
        else if (strcmp("2ONE",command)== 0){
            scanf("%s", arg1);
            scanf("%s", arg2);
            handle_2one(arg1, arg2);
        }
        else if (strcmp("STOP",command)== 0){
            handle_exit();
        }
        else{
            printf("Wrong command:\n");
        }
        
    }
    // closing the connected socket
    return 0;
}