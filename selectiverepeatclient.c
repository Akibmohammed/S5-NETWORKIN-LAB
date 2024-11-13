#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <time.h>

#define PORT 8080
#define MAX_SEQ 10

int check_corruption() {
    // Randomly simulate corruption
    return rand() % 2;
}

void send_ack(int socket, struct sockaddr_in *serverAddr, int seq, int isNegative) {
    char ack[1024];
    if (isNegative) {
        sprintf(ack, "nack %d", seq);
    } else {
        sprintf(ack, "ack %d", seq);
    }
    sendto(socket, ack, strlen(ack), 0, (struct sockaddr *)serverAddr, sizeof(*serverAddr));
    printf("Response/acknowledgement sent for message %d\n", seq);
}

int main() {
    int clientSocket;
    struct sockaddr_in serverAddr;
    socklen_t addrLen = sizeof(serverAddr);
    char buffer[1024];
    int expectedSeq = 0;

    // Seed for random corruption
    srand(time(0));

    // Create socket
    clientSocket = socket(AF_INET, SOCK_DGRAM, 0);
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr);

    printf("Client with individual acknowledgment scheme\n");

    // Main loop to receive messages
    while (expectedSeq < MAX_SEQ) {
        recvfrom(clientSocket, buffer, sizeof(buffer), 0, (struct sockaddr *)&serverAddr, &addrLen);
        buffer[strlen(buffer)] = '\0';

        int seqNum;
        sscanf(buffer, "server message :%d", &seqNum);
        printf("Message received from server: %s\n", buffer);

        // Check for corruption
        int isCorrupt = check_corruption();
        printf("Corruption status: %d\n", isCorrupt);

        if (isCorrupt) {
            send_ack(clientSocket, &serverAddr, seqNum, 1);  // Send negative acknowledgment
        } else {
            send_ack(clientSocket, &serverAddr, seqNum, 0);  // Send positive acknowledgment
            expectedSeq++;  // Move to next expected message
        }
    }

    close(clientSocket);
    return 0;
}
