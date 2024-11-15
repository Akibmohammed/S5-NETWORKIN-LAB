#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <time.h>

#define PORT 5000
#define MAX_FRAMES 10
#define WINDOW_SIZE 4
#define FRAME_SIZE 1024
#define TIMEOUT_SEC 2  // Timeout for ACK in seconds

int main() {
    int clientSocket;
    struct sockaddr_in serverAddr;
    char buffer[FRAME_SIZE];
    int frameAck[MAX_FRAMES] = {0};  
    int frameToSend = 0;             
    int framesSent = 0;              
    int ackReceived;                

    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket < 0) {
        perror("Client socket creation failed");
        exit(EXIT_FAILURE);
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        perror("Connection to server failed");
        close(clientSocket);
        exit(EXIT_FAILURE);
    }
    printf("Connected to server.\n");

    srand(time(0)); 

    while (framesSent < MAX_FRAMES) {
        int framesInWindow = 0;
        for (int i = frameToSend; i < MAX_FRAMES && framesInWindow < WINDOW_SIZE; i++) {
            if (!frameAck[i]) {
                sprintf(buffer, "Frame %d", i);
                send(clientSocket, buffer, strlen(buffer), 0);
                printf("Sent %s\n", buffer);
                framesInWindow++;
            }
        }

        for (int i = frameToSend; i < frameToSend + WINDOW_SIZE && i < MAX_FRAMES; i++) {
            if (frameAck[i]) continue;  

            fd_set readfds;
            struct timeval timeout;

            FD_ZERO(&readfds);
            FD_SET(clientSocket, &readfds);
            timeout.tv_sec = TIMEOUT_SEC;
            timeout.tv_usec = 0;

            int activity = select(clientSocket + 1, &readfds, NULL, NULL, &timeout);

            if (activity < 0) {
                perror("Select error");
                close(clientSocket);
                exit(EXIT_FAILURE);
            }

            if (activity == 0) {
                printf("Timeout for Frame %d, resending...\n", i);
                continue; 
            }

            int bytesReceived = recv(clientSocket, buffer, FRAME_SIZE, 0);
            if (bytesReceived > 0) {
                buffer[bytesReceived] = '\0';

                if (strncmp(buffer, "ACK ", 4) == 0) {
                    int ackFrame = atoi(buffer + 4);
                    printf("Acknowledgment received for Frame %d\n", ackFrame);

                    if (ackFrame >= frameToSend && ackFrame < frameToSend + WINDOW_SIZE) {
                        frameAck[ackFrame] = 1; 
                        framesSent++;
                    }

                    while (frameAck[frameToSend] && frameToSend < MAX_FRAMES) {
                        frameToSend++;
                    }
                }
            }
        }
    }

    printf("All frames sent and acknowledged.\n");
    close(clientSocket);
    return 0;
}
