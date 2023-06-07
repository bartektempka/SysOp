#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>
#include<sys/un.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<signal.h>
#include<sys/select.h>
#include<sys/time.h>
#include<pthread.h>

#include "shared.h"


int clients[MAX_CLIENTS];
int last_client = 0;
char client_names[MAX_CLIENTS][MAX_NAME_LEN];
int clients_stopped[MAX_CLIENTS] = {1};
char buffer[MAX_MSG_LEN] = {0};
pthread_mutex_t ping_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t ping_cond = PTHREAD_COND_INITIALIZER;
static int macos_pthread_mutex_timedlock(pthread_mutex_t *mutex, const struct 
timespec *abs_timeout)
{
    int rv;
    struct timespec remaining, slept, ts;

    remaining = *abs_timeout;
    while ((rv = pthread_mutex_trylock(mutex)) == EBUSY) {
        ts.tv_sec = 0;
        ts.tv_nsec = (remaining.tv_sec > 0 ? 10000000 : 
                     (remaining.tv_nsec < 10000000 ? remaining.tv_nsec : 
10000000));
        nanosleep(&ts, &slept);
        ts.tv_nsec -= slept.tv_nsec;
        if (ts.tv_nsec <= remaining.tv_nsec) {
            remaining.tv_nsec -= ts.tv_nsec;
        }
        else {
            remaining.tv_sec--;
            remaining.tv_nsec = (1000000 - (ts.tv_nsec - remaining.tv_nsec));
        }
        if (remaining.tv_sec < 0 || (!remaining.tv_sec && remaining.tv_nsec <= 
0)) {
            return ETIMEDOUT;
        }
    }

    return rv;
}
/*
 * A pthread_mutex_timedlock() impl for OSX/macOS, which lacks the
 * real thing.
 * NOTE: Unlike the real McCoy, won't return EOWNERDEAD, EDEADLK
 *       or EOWNERDEAD
 */
int handle_new_connection(int fd){
    int new_socket;
    
    if((new_socket = accept(fd, NULL, NULL)) < 0){
        perror("accept");
        return -1;
    } 
    if(last_client == MAX_CLIENTS){
        printf("Too many clients\n");
        close(new_socket);
        return -2;
    }
    if(-1 == fcntl(new_socket, F_SETFL, fcntl(new_socket, F_GETFL, 0) | O_NONBLOCK)) //nonblock set
    {
        perror("fcntl");
        exit(EXIT_FAILURE);
    }
    clients[last_client] = new_socket;
    clients_stopped[last_client] = 0;
    recv(new_socket, client_names[last_client], MAX_NAME_LEN,0);
    send(new_socket, "ping", 4, 0); //ping
    last_client++;
    printf("New client connected NAME: %s\n", client_names[last_client-1]);
    return 0;
}
void handle_list(int fd){
    memset(buffer, 0, MAX_MSG_LEN);
    strcat(buffer, "List of clients:\n");
    for (int i = 0;i< last_client;i++){
        strcat(buffer, client_names[i]);
        strcat(buffer, "\n");
    }
    send(fd, buffer, MAX_MSG_LEN, 0);
}
void handle_2all(int fd,char* msg){
    for (int i = 0;i< last_client;i++){
        if (clients[i] == fd || clients_stopped[i] == 1){
            continue;
        }
        send(clients[i], msg, MAX_MSG_LEN, 0);
    }
}
void handle_2one(int fd,char* msg){
    char* client_id = strtok(msg, " ");
    for(int i = 0;i< last_client;i++){
        if (strcmp(client_names[i], client_id) == 0 && clients_stopped[i] == 0){
            send(clients[i], strtok(NULL," "), MAX_MSG_LEN, 0);
            return;
        }
    }
    send(fd, "No such client", MAX_MSG_LEN, 0);
}
void handle_stop(int fd){
    shutdown(fd, SHUT_RDWR);
    close(fd);
    for (int i = 0;i< last_client;i++){
        if (clients[i] == fd){
            clients_stopped[i] = 1;
            printf("Client %s disconnected\n", client_names[i]);
        }
    }
    for (int i = 0;i< last_client;i++){
        if (clients_stopped[i] == 1){
            return;
        }
    }
    printf("ALL CLIENTS DISCONNECTED\n");
    printf("Server shutting down\n");
    exit(0);
    
}

void* pinging_thread(void* arg){
                
    while(1){

        for (int i = 0;i< last_client;i++){
            sleep(1);
            if (clients_stopped[i] == 1){
                continue;
            }
            printf("pinging %s\n", client_names[i]);
            send(clients[i], "PING", 4, 0);
            printf("waiting for response\n");

            // pthread_mutex_lock(&ping_mutex);
            if(macos_pthread_mutex_timedlock(&ping_mutex, &(struct timespec){time(NULL)+5, 0}) == ETIMEDOUT){
                printf("Client %s disconnected\n", client_names[i]);
                clients_stopped[i] = 1;
                shutdown(clients[i], SHUT_RDWR);
                close(clients[i]);
            }
            else{
                printf("Client %s responded\n", client_names[i]);
            }
            
        }

        sleep(5);
    }
}
void signal_handler(int signo){
    printf("SIGINT received\n");
    for (int i = 0;i< last_client;i++){
        if(clients_stopped[i] == 0){
            send(clients[i], "STOP", 4, 0);
            shutdown(clients[i], SHUT_RDWR);
            close(clients[i]);
        }
    }
    exit(0);
}

int main(int argc, char *argv[])
{
    int port = atoi(argv[1]);
    char socket_path[104];
    strcpy(socket_path, argv[2]);
    signal(SIGINT, signal_handler);

	int inet_fd,unix_fd, new_socket;
    struct sockaddr_in inet_address;
    struct sockaddr_un unix_address;
    int opt = 1;  
    // Creating socket file descriptor
    pthread_t pinging_thread_id;
    pthread_create(&pinging_thread_id, NULL, pinging_thread, NULL);
    if ((inet_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
    if(-1 == fcntl(inet_fd, F_SETFL, fcntl(inet_fd, F_GETFL, 0) | O_NONBLOCK)) //nonblock set
    {
        perror("fcntl");
        exit(EXIT_FAILURE);
    }
    if((unix_fd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0){
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
    if(-1 == fcntl(unix_fd, F_SETFL, fcntl(unix_fd, F_GETFL, 0) | O_NONBLOCK))
    {
        perror("fcntl");
        exit(EXIT_FAILURE);
    }
    
  
    // Set socket to be reusable
    if (setsockopt(inet_fd, SOL_SOCKET,
                   SO_REUSEADDR, &opt,
                   sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    inet_address.sin_family = AF_INET;
    inet_address.sin_addr.s_addr = INADDR_ANY;
    inet_address.sin_port = htons(port);
    
    unix_address.sun_family = AF_UNIX;
    strcpy(unix_address.sun_path, socket_path);
    unlink(socket_path);
    
    // Forcefully attaching socket to the port 8080
    if (bind(inet_fd, (struct sockaddr*)&inet_address,sizeof(inet_address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    if(bind(unix_fd, (struct sockaddr*)&unix_address, sizeof(unix_address)) < 0){
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(inet_fd, MAX_CLIENTS) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    if (listen(unix_fd, MAX_CLIENTS) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    
    //select setup bo na MACOS nie ma poll :(
    fd_set readfds;
    int max_fd;
    int activity;
    int message_flag = 0;
    int curr_fd;
    int message_type;

;
    while(1){
        max_fd = (inet_fd > unix_fd) ? inet_fd : unix_fd;
        FD_ZERO(&readfds);
        FD_SET(inet_fd, &readfds);
        FD_SET(unix_fd, &readfds);
        for(int i = 0; i < last_client; i++){
            if(clients_stopped[i] == 0){
                FD_SET(clients[i], &readfds);
                if (clients[i] > max_fd) 
                    max_fd = clients[i];
            }
        }

        printf("Waiting for activity\n");

        activity = select(max_fd + 1, &readfds, NULL, NULL, NULL);
        if(activity < 0){
            perror("select");
            exit(EXIT_FAILURE);
        }
        if(FD_ISSET(inet_fd, &readfds)){
            handle_new_connection(inet_fd);
        }
        else if(FD_ISSET(unix_fd, &readfds)){
            handle_new_connection(unix_fd);
        }
        else{ //check if new connection
            printf("NEW MESSAGE\n");
            for(int i = 0; i < last_client; i++){
                if(FD_ISSET(clients[i], &readfds)){
                    curr_fd = clients[i];
                    message_flag = 1;
                    printf("Message from: %s\n", client_names[i]);
                    break;
                }
            }
        }
        //why spcket is disconnected after accept?
        if (message_flag){ //if not check message
            if(recv(curr_fd, buffer, MAX_MSG_LEN,0)==-1){
                perror("read");
                exit(EXIT_FAILURE);
            }
            printf("Message: %s\n", buffer);
            
            message_type = buffer[0] - '0'; // to int 
            memmove(buffer, buffer+1, strlen(buffer)); //remove type
            switch(message_type){
                case LIST:
                    printf("LIST\n");
                    handle_list(curr_fd);
                    break;
                case TALL:
                    printf("TALL\n");
                    handle_2all(curr_fd, buffer);
                    break;
                case TONE:
                    printf("TONE\n");
                    handle_2one(curr_fd, buffer);
                    break;
                case STOP:
                    printf("STOP\n");
                    handle_stop(curr_fd);
                    break;
                case PING:
                    printf("PING back\n");
                    // pthread_mutex_lock(&ping_mutex);
                    // pthread_cond_signal(&ping_cond);
                    pthread_mutex_unlock(&ping_mutex);
                    break;
                default:
                    printf("Unknown message type\n");
                    break;
            }

        }



        message_flag = 0;
    }
    // closing the connected socket
    close(new_socket);
    // closing the listening socket
    shutdown(inet_fd, SHUT_RDWR);
    return 0;
}