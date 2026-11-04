#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>

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
    my_sock.sin_addr.s_addr = inet_addr("127.0.0.1");

    //connect
    int status_connect = connect(ret, (struct sockaddr *)&my_sock, sizeof(my_sock));
    if(status_connect == -1){
        printf("Connection Failed!!\n");
        close(ret);
        exit(-1);
    } else{
        printf("Connection Successful!!\n");
    }
    //send
    char msg[] = "Hello Server";
    ssize_t a = send(ret, msg, strlen(msg), 0);
    if(a == -1){
        printf("Send Failed!!\n");
        close(ret);
        exit(-1);
    } else{
        printf("Message Sent\n");
    }
    //recv
    char buff[1000];
    ssize_t b = recv(ret, buff, sizeof(buff)-1, 0);
    if(b == -1){
        printf("Receive Failed!!\n");
    } else if(b == 0){
        printf("Server disconnected!!\n");
    } else{
        buff[b] = '\0';
        printf("Received : %s\n", buff);
    }

    close(ret);
    return 0;
}
