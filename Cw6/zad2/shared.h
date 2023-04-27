#include <mqueue.h>
#include <fcntl.h>
#include <sys/stat.h>

#define MAX_CLIENTS 10
#define MAX_MSG_LEN 1024
#define SERVER_QUEUE_NAME "/server_queue"
enum msg_type{
    INIT = 1,
	LIST = 2,
	TALL = 3,
	TONE = 4,
	STOP = 5

};