#include <stdio.h>          // printf()
#include <stdlib.h>         // exit()
#include <unistd.h>         // close()
#include <sys/socket.h>     // socket(), bind(), recvfrom(), sendto()
#include <netinet/in.h>     // sockaddr_in
#include <string.h>         // strlen(), memset()
#include <arpa/inet.h>      // htons(), htonl()

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

    struct sockaddr_in my_sock;
    memset(&my_sock,0,sizeof(my_sock));
    my_sock.sin_family = AF_INET;
    my_sock.sin_port = htons(5000);
    my_sock.sin_addr.s_addr = htonl(INADDR_ANY);

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

    //recvfrom
    char buff[1000];
    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);
    ssize_t a = recvfrom(ret, buff, sizeof(buff),0, (struct sockaddr *)&client_addr, &client_len);
    if(a==-1)
    {
        printf("Receive Failed!!\n");
    }
    else
    {
        buff[a]='\0';
        printf("Received : %s\n",buff);
    }

    //sendto
    char msg[]="hello!!";
    ssize_t b = sendto(ret, msg, strlen(msg), 0, (const struct sockaddr *)&client_addr, client_len);
    if(b==-1)
    {
        printf("Send Failed!!\n");
    }
    else
    {
        printf("Reply Sent\n");
    }

    //close
    close(ret);

    return 0;
}