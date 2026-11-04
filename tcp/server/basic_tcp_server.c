#include <stdio.h>          
#include <stdlib.h>        
#include <unistd.h>        
#include <sys/socket.h>     
#include <netinet/in.h>     
#include <string.h>        
#include <arpa/inet.h>      

int main()
{
    int ret = socket(AF_INET, SOCK_STREAM, 0);
    if(ret == -1)
    {
        perror("Socket creation failed");
        exit(-1);
    }
    printf("Socket created successfully\n");

    struct sockaddr_in my_sock;
    memset(&my_sock, 0, sizeof(my_sock));
    my_sock.sin_family = AF_INET;
    my_sock.sin_port = htons(5000);
    my_sock.sin_addr.s_addr = htonl(INADDR_ANY);

    int status = bind(ret, (const struct sockaddr *)&my_sock, sizeof(my_sock));
    if(status == -1){
        perror("Bind Failed");
        close(ret);
        exit(-1);
    } else {
        printf("Bind Successful!!\n");
    }

    int status_listen = listen(ret, 5);
    if(status_listen == -1){
        perror("Listen Failed");
        close(ret);
        exit(-1);
    } else {
        printf("Listen Successful!!\n");
    }
    
    while(1){
        struct sockaddr_in client_sock;
        socklen_t clientsock_len = sizeof(client_sock); 
        int status_accept = accept(ret, (struct sockaddr *)&client_sock, &clientsock_len);
        if(status_accept == -1){
            perror("Accept Failed");
            continue;
        } else {
            printf("Client connected: %s:%d\n", 
                   inet_ntoa(client_sock.sin_addr), 
                   ntohs(client_sock.sin_port));
        }

        while(1){
            char buff[1000];
            ssize_t a = recv(status_accept, buff, sizeof(buff) - 1, 0);
            
            if(a < 0){
                perror("Receive Failed");
                break; 
            } else if (a == 0) {
                printf("Client disconnected.\n");
                break; // Exit inner loop on clean close
            } else {
                buff[a] = '\0';
                printf("Received : %s\n", buff);
            }

            char msg[] = "hello!!";
            ssize_t b = send(status_accept, msg, strlen(msg), 0);
            if(b == -1){
                perror("Send Failed");
                break;
            } else {
                printf("Reply Sent\n");
            }
        }
 
        close(status_accept); 
    }

    close(ret);
    return 0;
}