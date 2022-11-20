#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <netinet/in.h>
//#define SERVER_NAME "micro_httpd"
#define SERVER_URL "127.0.0.1"
#define PROTOCOL "HTTP/1.0"
#define BUF_SIZE 1024
#define LISTENQ 128

int client_socket;
char msg[100];

pthread_mutex_t wlock;
pthread_mutex_t mlock;
pthread_cond_t cond;

void *worker(void *ptr){
    char method[32], file[32], protocol[32];
    char buf[BUF_SIZE];
    char tmp;
    int num;
    int i;
    unsigned int count;
    FILE *f;
    while(1){
        pthread_cond_wait(&cond, &wlock);
        if (sscanf(msg, "%[^ ] %[^ ] %[^ ]", method, file, protocol) != 3){
            sprintf(buf, "400 Bad Request\n");
        }   
        
        f = fopen(file, "r");
        if ( f == (FILE*) 0 ){
            sprintf(buf, "403 Forbidden\n");
        }
	    
        for(count = 0; fgets(buf, BUF_SIZE, f) != NULL; count++){}
        fclose(f);
        
        send(client_socket, &count, sizeof(count), 0);
        f = fopen(file, "r");
        
        for(i=0; i<count; i++){
            fgets(buf, BUF_SIZE, f);
            send(client_socket, buf, BUF_SIZE, 0);
        }
        
        fclose(f);
        printf("COMPLETE\n");
    }
}

int main(int argc, char **argv){
    int port;
    int threads_num;
    int server_socket;
    int addrlen;
    int i;
    
    struct sockaddr_in server_addr;
    struct sockaddr_in client_addr;
    pthread_t tid;
     
    if(argc != 3){
		printf("Usage: ./server_mw [port num] [the number of threads]\n");
		return -1;
	}
    port = atoi(argv[1]); //printf("port: %d\n", port);
	threads_num = atoi(argv[2]);
    pthread_mutex_init(&wlock, NULL);
    pthread_cond_init(&cond, NULL);
    if((server_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        printf("Error: Unable to open socket in server.\n");
        return -1;
    }
    memset((char *)&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(port);

// setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&one, sizeof(one));

    if(bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0){
        printf("Error: Unable to bind socket in server.\n");
        return -1;
    }
    if(listen(server_socket, LISTENQ) < 0){
		printf("Error: Unable to listen socket in server\n");
		return -1;
	}
    

    for(i=0; i<threads_num; i++){
        pthread_create(&tid, NULL, worker, NULL);
        pthread_detach(tid);
    }

    addrlen = sizeof(client_addr);
    client_socket = accept(server_socket, (struct sockaddr *)&server_addr, &addrlen);
    if(client_socket < 0){ 
        printf("Error: Connection to client failed in server.\n");
        return -1;
    }

    while(1){
        recv(client_socket, msg, sizeof(msg), 0);
        pthread_cond_broadcast(&cond);
    }

    close(server_socket);
    close(client_socket);

    return 0;
}

