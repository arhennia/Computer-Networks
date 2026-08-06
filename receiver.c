#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>

int main()
{
    // Create UDP socket
    int ret = socket(AF_INET, SOCK_DGRAM, 0);
    if(ret == -1)
    {
        printf("Socket creation failed!!\n");
        exit(-1);
    }
    printf("Socket created successfully\n");

    // Server address
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(5000);

    // Server IP Address
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    // Message to send
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

    // Receive reply
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

    close(ret);
    return 0;
}