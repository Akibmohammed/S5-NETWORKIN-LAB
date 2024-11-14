#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <time.h>

#define PORT 8080
#define WINDOW_SIZE 4
#define TOTAL_FRAMES 10

int main() {
    int sockfd;
    struct sockaddr_in serverAddr;
    socklen_t addr_size;
    int base = 0;
    int nextFrameToSend = 0;
    int ack;
    int frames[TOTAL_FRAMES];

    // Initialize frames with sequence numbers
    for (int i = 0; i < TOTAL_FRAMES; i++) {
        frames[i] = i;
    }

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    printf("Client started. Sending frames...\n");

    while (base < TOTAL_FRAMES) {
        // Send frames in the window
        while (nextFrameToSend < base + WINDOW_SIZE && nextFrameToSend < TOTAL_FRAMES) {
            printf("Sending frame: %d\n", frames[nextFrameToSend]);
            sendto(sockfd, &frames[nextFrameToSend], sizeof(int), 0, (struct sockaddr *)&serverAddr, sizeof(serverAddr));
            nextFrameToSend++;
        }

        // Receive ACK
        addr_size = sizeof(serverAddr);
        int recv_status = recvfrom(sockfd, &ack, sizeof(ack), 0, (struct sockaddr *)&serverAddr, &addr_size);

        if (recv_status > 0) {
            printf("Received ACK for frame: %d\n\n", ack);
            base = ack + 1;  // Slide the window if ACK received
        } else {
            printf("Timeout or error. Resending from frame %d\n\n", base);
            nextFrameToSend = base;  // Reset to base for retransmission
        }
    }

    printf("All frames sent successfully.\n");
    close(sockfd);
    return 0;
}
