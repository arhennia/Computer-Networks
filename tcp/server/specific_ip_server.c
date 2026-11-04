#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

#define PORT 6060
#define BUF_SIZE 1024

int main() {

    // 1. socket() - create the listening socket
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        printf("Socket creation failed.\n");
        return 0;
    } else {
        printf("Socket creation successful.\n");
    }
    
    // 2. bind() - exactly same as UDP: attach an address to the socket
    struct sockaddr_in servaddr;
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    // servaddr.sin_addr.s_addr = INADDR_ANY;
    inet_aton("100.81.134.57", &servaddr.sin_addr); //network layer equivalent of htons and ntohs

    if (bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) == -1) {
        printf("Bind failed.\n");
        return 0;
    } else {
        printf("Bind successful.\n");
    }

    // 3. listen() - mark socket as passive, ready to accept connections
    //    backlog = 5 -> up to 5 pending connections can wait in the queue
    if (listen(sockfd, 5) == -1) {
        printf("Listen failed.\n");
        return 0;
    } else {
        printf("Listening on port %d...\n", PORT);
    }

    // 4. accept() - pick up a pending connection request from the queue
    //    blocks (halts) here until a client connects.
    //    returns a NEW socket fd dedicated to this one client.
    struct sockaddr_in clientaddr;
    socklen_t clientlen = sizeof(clientaddr);

    int newsockfd = accept(sockfd, (struct sockaddr *)&clientaddr, &clientlen);
    if (newsockfd == -1) {
        printf("Accept failed.\n");
        return 0;
    } else {
        printf("Connection established with %s:%d\n",
               inet_ntoa(clientaddr.sin_addr),
               ntohs(clientaddr.sin_port));
    }

    char sendbuf[BUF_SIZE];
    char recvbuf[BUF_SIZE];

    while (1) {

        // 5. recv() - receive data on the connected socket (newsockfd)
        memset(recvbuf, 0, BUF_SIZE);
        int n = recv(newsockfd, recvbuf, BUF_SIZE, 0);
        if (n <= 0) {
            printf("Client disconnected.\n");
            break;
        }
        recvbuf[n] = '\0';
        printf("Client: %s\n", recvbuf);

        if (strcmp(recvbuf, "bye") == 0 || strcmp(recvbuf, "exit") == 0) {
            printf("Client ended the conversation. Closing.\n");
            break;
        }

        // 6. send() - reply back on the connected socket
        printf("You: ");
        fgets(sendbuf, BUF_SIZE, stdin);
        sendbuf[strcspn(sendbuf, "\n")] = '\0';

        send(newsockfd, sendbuf, strlen(sendbuf), 0);

        if (strcmp(sendbuf, "bye") == 0 || strcmp(sendbuf, "exit") == 0) {
            printf("You ended the conversation. Closing.\n");
            break;
        }
    }

    // 7. close() - close both the connected socket and the listening socket
    close(newsockfd);
    close(sockfd);
    return 0;
}