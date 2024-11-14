#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>

#define PORT 8778

int main() {
    // Create UDP socket
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("Socket creation failed");
        exit(1);
    }

    // Server address configuration
    struct sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    // Input message to send to the server
    printf("Enter the message to send to the server: ");
    char data[1024];
    scanf(" %[^\n]", data);  // Read until newline, allowing spaces

    // Send the message to the server
    sendto(sockfd, data, strlen(data), 0, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

    // Receive a message from the server
    int len = sizeof(serv_addr);
    int bytes_received = recvfrom(sockfd, data, sizeof(data) - 1, 0, (struct sockaddr*)&serv_addr, &len);

    // Null-terminate the received data
    if (bytes_received >= 0) {
        data[bytes_received] = '\0';
        printf("\nMessage from server: %s\n", data);
    } else {
        perror("Receive failed");
    }

    // Close the socket
    close(sockfd);
    return 0;
}
