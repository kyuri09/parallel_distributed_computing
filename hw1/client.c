#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>

#define N 1
#define ACCESS 1
char file_list[5][100];
pthread_mutex_t lock;
int client;
pthread_cond_t cond;

void *worker(void *a)
{
    unsigned int cnt,i,j;
    char buf[100];
    char buffer[1024];
    clock_t start, end;

    srand(time(NULL));
    start = clock();
    int num = rand()%N;
    for(i=0;i<ACCESS;i++)
    {
        pthread_mutex_lock(&lock);
        
        sprintf(buf, "GET %s HTTP/1.0", file_list[num]);
        printf("%s\n",buf);
        send(client, buf, sizeof(buf), 0);
        
        recv(client, &cnt, sizeof(cnt), 0);
        for(j=0; j<cnt; j++){
            recv(client, buffer, 1024, 0);
        }

        pthread_mutex_unlock(&lock);
    }

    end = clock();
    printf("Execution Time of Each Thread : %lf\n", (double)(end-start)/CLOCKS_PER_SEC);


}
int main(int argc, char** argv)
{
    int p;
    FILE *fp;
    pthread_t tid;
    char filename[16];
    struct sockaddr_in server;
    int one,i;
    if (argc != 4)
	{
		fprintf(stderr, "usage: ./client [Port] [num of threads] [file name]\n");
		exit(0);
	}

	int port=atoi(argv[1]);
	int thread_cnt=atoi(argv[2]);
	strcpy(filename,argv[3]);
   // printf("%s\n",filename);
    pthread_mutex_init(&lock, NULL);
    pthread_cond_init(&cond, NULL);
    fp=fopen(filename,"r");

    for(i=0;i<N;i++)
    {
        fscanf(fp,"%s",file_list[i]);
            //printf("%s\n",file_list[i]);

    }
    fclose(fp);

    if((client = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        printf("Error: Unable to open socket in server.\n");
        exit(1);
    }
    
    memset(&server, 0, sizeof(server));
    server.sin_family=AF_INET;
	server.sin_addr.s_addr=inet_addr("127.0.0.1");
	server.sin_port=htons(port);

    if(connect(client, (struct sockaddr *)&server, sizeof(server)) < 0){
        printf("Error: Connection to client failed in server.\n");
        exit(1);
    }

    // create threads
    for(i=0;i<thread_cnt;i++)
    {
		pthread_create(&tid,NULL,worker,NULL);
        pthread_detach(tid);
    }

    while(1){ }
    close(client);

}