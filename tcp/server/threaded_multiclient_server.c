#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <pthread.h>

void *start_thread(void *arg){
    int data_sock = *(int *)arg;
    free(arg);
    //recv
    char buff[1000];
    ssize_t a = recv(data_sock, buff, sizeof(buff)-1, 0);
    if(a == -1){
        printf("Receive Failed!!\n");
    } else{
        buff[a] = '\0';
        printf("Received : %s\n", buff);
    }
    //send
    char msg[] = "hi";
    ssize_t b = send(data_sock, msg, strlen(msg), 0);
    if(b == -1){
        printf("Send Failed!!\n");
    } else{
        printf("Reply Sent\n");
    }
    close(data_sock);
    return NULL;
}

int main(){
    //socket
    int ret = socket(AF_INET, SOCK_STREAM, 0);
    if(ret == -1){
        printf("Socket creation failed!!\n");
        exit(-1);
    }
    printf("Socket created successfully\n");
    struct sockaddr_in my_sock;
    memset(&my_sock, 0, sizeof(my_sock));
    my_sock.sin_family = AF_INET;
    my_sock.sin_port = htons(5000);
    my_sock.sin_addr.s_addr = htonl(INADDR_ANY);

    //bind
    int status_bind = bind(ret, (struct sockaddr *)&my_sock, sizeof(my_sock));
    if(status_bind == -1){
        printf("Bind failed!!\n");
        close(ret);
        exit(-1);
    } else {
        printf("Bind successful!!\n");
    }
    //listen
    int status_listen = listen(ret, 5);
    if(status_listen == -1){
        printf("Listen Failed!!\n");
        close(ret);
        exit(-1);
    } else{
        printf("Listen Successful!!\n");
    }

    while(1){
        //accept
        struct sockaddr_in client_addr;
        socklen_t clientsock_len = sizeof(client_addr);
        int status_accept = accept(ret, (struct sockaddr *)&client_addr, &clientsock_len);
        if(status_accept == -1){
            printf("Accept failed!\n");
        } else {
            printf("Accept successful! client connected!\n\n");
        }

        //pthread_create
        int *data_sock = malloc(sizeof(int));
        if(data_sock == NULL){
            printf("Memory allocation failed!!\n");
            close(status_accept);
        }
        *data_sock = status_accept;
        pthread_t tid;
        if(pthread_create(&tid, NULL, start_thread, data_sock) != 0){
            printf("Thread creation failed!!\n");
            close(*data_sock);
            free(data_sock);
        }
        pthread_join(tid, NULL);
    }

    close(ret);
    return 0;
}
