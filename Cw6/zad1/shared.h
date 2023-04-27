#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <signal.h>
#include <sys/msg.h>
#include <sys/ipc.h>

#define MAX_CLIENTS 10
#define MAX_MSG_LEN 1024
#define PROJECT_ID 1


struct msg_buff {
	long        msg_type;
    struct data{
            key_t       client_key;
            int         client_queue_id;
            int         client_id;
            char        msg[MAX_MSG_LEN];
        }data;
};
enum msg_type{
    INIT = 1,
	LIST = 2,
	TALL = 3,
	TONE = 4,
	STOP = 5

};