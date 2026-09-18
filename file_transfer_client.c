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
    my_sock.sin_addr.s_addr = htonl(INADDR_ANY);
    // inet_aton("100.81.134.57", &my_sock.sin_addr); //translation shit

    //connect
    int status_connect = connect(ret, (const struct sockaddr *)&server_sock, sizeof(server_sock));
    if(status_connect == -1){
        printf("Failed to connect!!\n");
        exit(-1);
    } else {
        printf("Successfully connected!!\n\n");
    }

    char buff[1000];

    FILE *fp;
    fp = fopen("result.txt","r");

    while(1){
        ssize_t a = recv(status_connect, buff, sizeof(buff), 0);
        if(a <= 0){
            printf("Client receive failed!!\n");
            break;
        }
        size_t b = fwrite(buff, sizeof(char), sizeof(buff), fp);
        if(b == -1){
            printf("write Failed");
            break;
            } else {
                printf("write successful");
            }
    }

    close(ret);
    return 0;
}