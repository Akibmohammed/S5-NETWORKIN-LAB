#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdbool.h>

#define WINDOW_SIZE 4

int main() {
    char buff[1024];
    int clientSocket, numFrames;
    struct sockaddr_in serverAddr;
    bool ackReceived[1024] = {false};  

    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(9999);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    connect(clientSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr));

    printf("Enter the number of frames to be sent: ");
    scanf("%d", &numFrames);

    int base = 0, nextFrame = 0;

    while (base < numFrames) {
        // Send frames within the window
        while (nextFrame < base + WINDOW_SIZE && nextFrame < numFrames) {
            if (!ackReceived[nextFrame]) { // Send only if not acknowledged
                sprintf(buff, "Frame %d", nextFrame);
                send(clientSocket, buff, strlen(buff), 0);
                printf("Sent frame %d\n", nextFrame);
            }
            nextFrame++;
        }
        int bytesReceived = recv(clientSocket, buff, sizeof(buff), 0);
        if (bytesReceived > 0) {
            buff[bytesReceived] = '\0';
            int ackNumber;
            sscanf(buff, "Ack %d", &ackNumber);
            printf("Ack received: %s\n", buff);

            // Mark frame as acknowledged
            if (ackNumber >= 0 && ackNumber < numFrames) {
                ackReceived[ackNumber] = true;

                // Slide window if the base frame is acknowledged
                while (ackReceived[base] && base < numFrames) {
                    base++;
                }
            }
        }
    }

    close(clientSocket);
    return 0;
}
