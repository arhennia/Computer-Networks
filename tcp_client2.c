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

    struct sockaddr_in server_sock;
    memset(&server_sock, 0, sizeof(server_sock));
    server_sock.sin_family = AF_INET;
    server_sock.sin_port = htons(5000);
    server_sock.sin_addr.s_addr = inet_addr("127.0.0.1");

    int status_connect = connect(ret, (const struct sockaddr *)&server_sock, sizeof(server_sock));
    if(status_connect == -1){
        printf("Failed to connect!!\n");
        exit(-1);
    } else {
        printf("Successfully connected!!\n\n");
    }

    char msg[1000];
    char buff[1000];

    while(1) {
        // send user input to server
        printf("Client (you): ");
        fgets(msg, sizeof(msg), stdin);

        ssize_t a = send(ret, msg, strlen(msg), 0);
        if(a == -1){
            printf("Send Failed!!\n");
            break;
        }

        // check if client typed exit
        if(strncmp(msg, "exit", 4) == 0){
            printf("Exit command sent. Terminating communication...\n");
            break;
        }

        // recv reply from server
        ssize_t b = recv(ret, buff, sizeof(buff) - 1, 0);
        if(b <= 0){
            printf("Server disconnected or receive failed!!\n");
            break;
        }

        buff[b] = '\0';
        printf("Reply from server : %s", buff);

        // check if server sent exit
        if(strncmp(buff, "exit", 4) == 0){
            printf("Server sent exit command. Terminating communication...\n");
            break;
        }
    }

    close(ret);
    return 0;
}