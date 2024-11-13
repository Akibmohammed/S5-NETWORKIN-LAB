#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <time.h>

#define PORT 8080
#define MAX_SEQ 10  // Maximum sequence number

void send_message(int socket, struct sockaddr_in *clientAddr, int seq, int isRetransmit) {
    char message[1024];
    sprintf(message, "server message :%d", seq);

    // If it is a retransmission, change message to reflect this
    if (isRetransmit) {
        sprintf(message, "reserver message :%d", seq);
    }

    sendto(socket, message, strlen(message), 0, (struct sockaddr *)clientAddr, sizeof(*clientAddr));
    printf("Message sent to client : %s\n", message);
}

int main() {
    int serverSocket;
    struct sockaddr_in serverAddr, clientAddr;
    socklen_t addrLen = sizeof(clientAddr);
    char buffer[1024];
    int windowStart = 0;

    // Create socket
    serverSocket = socket(AF_INET, SOCK_DGRAM, 0);
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    bind(serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr));

    printf("Server up\nSelective Repeat scheme\n");

    // Seed for random corruption
    srand(time(0));

    // Main loop to send messages
    for (int i = 0; i < MAX_SEQ; i++) {
        send_message(serverSocket, &clientAddr, i, 0);

        // Wait for acknowledgment
        recvfrom(serverSocket, buffer, sizeof(buffer), 0, (struct sockaddr *)&clientAddr, &addrLen);
        int ackSeq;
        sscanf(buffer, "ack %d", &ackSeq);

        if (ackSeq != i) {
            printf("Corrupt message acknowledgement (msg %d)\n", i);
            send_message(serverSocket, &clientAddr, i, 1);  // Retransmit message
        } else {
            printf("Acknowledgement of %d received\n", i);
        }
    }

    close(serverSocket);
    return 0;
}
