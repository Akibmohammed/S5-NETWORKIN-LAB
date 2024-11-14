#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>

#define PORT 8778

int main() {
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("Socket creation failed");
        exit(1);
    }
    struct sockaddr_in serv_addr, client_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr))
    printf("Server is running and waiting for messages...\n");

    while (1) {
        char buffer[1024];  
        int len = sizeof(client_addr);
        int bytes_received = recvfrom(sockfd, buffer, sizeof(buffer) - 1, 0, (struct sockaddr *)&client_addr, &len);

        if (bytes_received >= 0) {
            buffer[bytes_received] = '\0'; 
            printf("Received message from client: %s\n", buffer);
          
            char response[] = "Message received by server";
            sendto(sockfd, response, strlen(response), 0, (struct sockaddr *)&client_addr, len);
        } else {
            perror("Receive failed");
        }
    }
    close(sockfd);
    return 0;
}
