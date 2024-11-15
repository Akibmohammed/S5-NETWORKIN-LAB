#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <time.h>

#define PORT 8080
#define MAX_SEQ 10

int main() {
    int clientSocket;
    struct sockaddr_in serverAddr;
    socklen_t addrLen = sizeof(serverAddr);
    char buffer[1024];
    int expectedSeq = 0;
    
    srand(time(0));

    clientSocket = socket(AF_INET, SOCK_DGRAM, 0);
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr);

    printf("Client with individual acknowledgment scheme\n");

    while (expectedSeq < MAX_SEQ) {
        // Receive message from server
        recvfrom(clientSocket, buffer, sizeof(buffer), 0, (struct sockaddr *)&serverAddr, &addrLen);
        buffer[strlen(buffer)] = '\0';

        int seqNum;
        sscanf(buffer, "server message :%d", &seqNum);
        printf("Message received from server: %s\n", buffer);

        int isCorrupt = rand() % 2;
        printf("Corruption status: %d\n", isCorrupt);

        char ack[1024];
        if (isCorrupt) {
            sprintf(ack, "nack %d", seqNum);
            printf("Sending negative acknowledgment for message %d\n", seqNum);
        } else {
            sprintf(ack, "ack %d", seqNum);
            printf("Sending positive acknowledgment for message %d\n", seqNum);
            expectedSeq++;
        }

        sendto(clientSocket, ack, strlen(ack), 0, (struct sockaddr *)&serverAddr, sizeof(serverAddr));
        printf("Response/acknowledgement sent for message %d\n", seqNum);
    }

    close(clientSocket);
    return 0;
}
