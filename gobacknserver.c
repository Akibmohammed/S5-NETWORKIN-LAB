#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>

#define PORT 8080
#define WINDOW_SIZE 4

int main() {
    int sockfd;
    struct sockaddr_in serverAddr, clientAddr;
    socklen_t addr_size;
    int expectedFrame = 0;
    int ack;

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    if (bind(sockfd, (const struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0) {
        perror("Bind failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d\n", PORT);

    while (1) {
        int frame;
        addr_size = sizeof(clientAddr);

        recvfrom(sockfd, &frame, sizeof(frame), 0, (struct sockaddr *)&clientAddr, &addr_size);

        if (frame == expectedFrame) {
            printf("Received frame: %d\n", frame);
            ack = frame;
            expectedFrame++;
        } else {
            printf("Out-of-order frame: %d, expected: %d. Resending ACK for %d\n", frame, expectedFrame, expectedFrame - 1);
            ack = expectedFrame - 1;  
        }

        sendto(sockfd, &ack, sizeof(ack), 0, (struct sockaddr *)&clientAddr, addr_size);
        printf("Sent ACK for frame: %d\n\n", ack);
    }

    close(sockfd);
    return 0;
}
