#include <stdio.h>          
#include <stdlib.h>        
#include <unistd.h>        
#include <sys/socket.h>     
#include <netinet/in.h>     
#include <string.h>        
#include <arpa/inet.h>      

int main()
{
    // socket
    int ret = socket(AF_INET, SOCK_STREAM, 0);
    if(ret == -1)
    {
        printf("Socket creation failed!!\n");
        exit(-1);
    }
    printf("Socket created successfully\n");

    struct sockaddr_in my_sock;
    memset(&my_sock, 0, sizeof(my_sock));
    my_sock.sin_family = AF_INET;
    my_sock.sin_port = htons(5000);
    // my_sock.sin_addr.s_addr = htonl(INADDR_ANY);
    inet_aton("100.81.134.57", &my_sock.sin_addr); //translation shit

    // bind
    int status = bind(ret, (const struct sockaddr *)&my_sock, sizeof(my_sock));
    if(status == -1){
        printf("Bind Failed!!\n");
        exit(-1);
    } else {
        printf("Bind Successful!!\n");
    }

    // listen
    int status_listen = listen(ret, 5);
    if(status_listen == -1){
        printf("Listen Failed!!\n");
        exit(-1);
    } else {
        printf("Listen Successful!!\n");
    }

    // accept
    struct sockaddr_in client_sock;
    socklen_t clientsock_len = sizeof(client_sock);
    int status_accept = accept(ret, (struct sockaddr *)&client_sock, &clientsock_len);
    if(status_accept == -1){
        printf("Accept Failed!!\n");
        exit(-1);
    } else {
        printf("Accept Successful!! Client connected.\n\n");
    }

    char buff[1000];
    char msg[1000];

    // continuous message exchange loop
    while(1) {
        // recv message from client
        ssize_t a = recv(status_accept, buff, sizeof(buff) - 1, 0);
        if(a <= 0){
            printf("Client disconnected or receive failed!!\n");
            break;
        }

        buff[a] = '\0'; 
        printf("Received : %s", buff);

        // check if client sent exit
        if(strncmp(buff, "exit", 4) == 0){
            printf("Client sent exit command. Terminating communication...\n");
            break;
        }

        // send user input to client
        printf("Server (you): ");
        fgets(msg, sizeof(msg), stdin);

        // Fixed: Use status_accept socket descriptor instead of ret
        ssize_t b = send(status_accept, msg, strlen(msg), 0);
        if(b == -1){
            printf("Send Failed!!\n");
            break;
        }

        // check if server typed exit
        if(strncmp(msg, "exit", 4) == 0){
            printf("Exit command sent. Terminating communication...\n");
            break;
        }
    }

    close(status_accept);
    close(ret);
    return 0;
}