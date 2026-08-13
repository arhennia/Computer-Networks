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
    int ret = socket(AF_INET, SOCK_DGRAM, 0);
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

    //bind
    int status = bind(ret, (const struct sockaddr *)&my_sock, sizeof(my_sock));
    if(status==-1)
    {
        printf("Bind Failed!!\n");
    }
    else
    {
        printf("Bind Successful!!\n");
    }

    //sendto
    char msg[] = "Hello Server!!";
    ssize_t a = sendto(ret, msg, strlen(msg), 0, (const struct sockaddr *)&server_addr, sizeof(server_addr));
    if(a == -1)
    {
        printf("Send Failed!!\n");
    }
    else
    {
        printf("Message Sent Successfully\n");
    }

    //recvfrom
    char buff[1000];
    socklen_t server_len = sizeof(server_addr);
    ssize_t b = recvfrom(ret, buff, sizeof(buff), 0, (struct sockaddr *)&server_addr, &server_len);
    if(b == -1)
    {
        printf("Receive Failed!!\n");
    }
    else
    {
        buff[b] = '\0';
        printf("Reply from server : %s\n", buff);
    }
    
    //close
    close(ret);
    return 0;
}