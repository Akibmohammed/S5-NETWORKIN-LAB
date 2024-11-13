#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <time.h>

#define PORT 8080
#define MAX_SEQ 10  // Maximum sequence number

int main() {
    int serverSocket;
    struct sockaddr_in serverAddr, clientAddr;
    socklen_t addrLen = sizeof(clientAddr);
    char buffer[1024];
    int windowStart = 0;
  
    serverSocket = socket(AF_INET, SOCK_DGRAM, 0);
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    bind(serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr));

    printf("Server up\nSelective Repeat scheme\n");

    srand(time(0));

    for (int i = 0; i < MAX_SEQ; i++) {
        char message[1024];
        sprintf(message, "server message :%d", i);

        // Send message to client
        sendto(serverSocket, message, strlen(message), 0, (struct sockaddr *)&clientAddr, sizeof(clientAddr));
        printf("Message sent to client : %s\n", message);

        // Wait for acknowledgment
        recvfrom(serverSocket, buffer, sizeof(buffer), 0, (struct sockaddr *)&clientAddr, &addrLen);
        int ackSeq;
        sscanf(buffer, "ack %d", &ackSeq);

        // Check if acknowledgment is for the correct message
        if (ackSeq != i) {
            printf("Corrupt message acknowledgement (msg %d)\n", i);

            // Prepare retransmission message
            sprintf(message, "reserver message :%d", i);

            // Retransmit message to client
            sendto(serverSocket, message, strlen(message), 0, (struct sockaddr *)&clientAddr, sizeof(clientAddr));
            printf("Retransmitting message to client : %s\n", message);
        } else {
            printf("Acknowledgement of %d received\n", i);
        }
    }

    close(serverSocket);
    return 0;
}
