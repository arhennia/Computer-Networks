#include <stdio.h>          
#include <stdlib.h>        
#include <unistd.h>         
#include <sys/socket.h>     
#include <netinet/in.h>     
#include <string.h>         
#include <arpa/inet.h>      


int main()
{
    //udp socket
    int ret = socket(AF_INET, SOCK_STREAM, 0);
    if(ret == -1)
    {
        printf("Socket creation failed!!\n");
        exit(-1);
    }
    printf("Socket created successfully\n");

    struct sockaddr_in my_sock;
    memset(&my_sock,0,sizeof(my_sock));
    my_sock.sin_family = AF_INET;
    my_sock.sin_port = htons(5000);
    my_sock.sin_addr.s_addr = htonl(INADDR_ANY);


    //bind
    int status = bind(ret, (const struct sockaddr *)&my_sock, sizeof(my_sock));
    if(status==-1){
        printf("Bind Failed!!\n");
    }
    else{
        printf("Bind Successful!!\n");
    }

    //listen
    int status_listen = listen(ret, 5);
    if(status_listen == -1){
        printf("Listen Failed!!\n");
    }else{
        printf("Listen Successful!!\n");
    }

    //accept
    struct sockaddr_in client_sock;
    int clientsock_len = sizeof(client_sock);
    int status_accept = accept(ret, (struct sockaddr *)&client_sock, &clientsock_len);
    if(status_accept == -1){
        printf("Accept Failed!!\n");
        exit(-1);
    }else{
        printf("Accept Successful!!\n");
    }


    //recv
    char buff[1000];
    ssize_t a = recv(status_accept, buff, sizeof(buff), 0);
    // printf("Port number of sender: %d\n",ntohs(client_addr.sin_port));
    if(a==-1){
        printf("Receive Failed!!\n"); exit(-1);
    } else{
        buff[a]='\0';
        printf("Received : %s\n",buff);
    }

    //send
    char msg[]="hello!!";
    ssize_t b = send(ret, msg, strlen(msg), 0);
    if(b==-1){
        printf("Send Failed!!\n"); exit(-1);
    } else {
        printf("Reply Sent\n");
    }

    close(ret);
    return 0;
}