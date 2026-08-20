#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>

int main()
{
    //socket
    int ret = socket(AF_INET, SOCK_STREAM, 0);
    if(ret == -1)
    {
        printf("Socket creation failed!!\n");
        exit(-1);
    }
    printf("Socket created successfully\n");

    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(5000);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    //connect
    struct sockaddr_in server_sock;
    memset(&server_sock, 0, sizeof(server_sock));
    server_sock.sin_family = AF_INET;
    server_sock.sin_port = htons(5000);
    server_sock.sin_addr.s_addr = inet_addr("127.0.0.1");

    int status_connect = connect(ret, (const struct sockaddr *)&server_sock, sizeof(server_sock));
    if(status_connect == -1){
        printf("Failed to connect!!\n");
        exit(-1);
    } else{
        printf("Successfully connected!!\n");
    }

    //send
    char msg[] = "Hello Server!!";
    ssize_t a = send(ret, msg, strlen(msg), 0);
    if(a == -1){
        printf("Send Failed!!\n"); exit(-1);
    } else{
        printf("Message Sent Successfully\n");
    }

    //recv
    char buff[1000];
    ssize_t b = recv(ret, buff, sizeof(buff), 0);
    if(b == -1){
        printf("Receive Failed!!\n"); exit(-1);
    } else{
        buff[b] = '\0';
        printf("Reply from server : %s\n", buff);
    }

    close(ret);
    return 0;
}