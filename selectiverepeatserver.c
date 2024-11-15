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

int is_frame_corrupted() {
    return rand() % 5 == 0;  
}
void random_delay() {
    int delay = rand() % 1000; 
    usleep(delay * 1000); 
}

int main() {
    int serverSocket, clientSocket;
    struct sockaddr_in serverAddr;
    char buffer[FRAME_SIZE];
    int framesReceived[MAX_FRAMES] = {0}; 

    srand(time(0)); 

    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket < 0) {
        perror("Server socket creation failed");
        exit(EXIT_FAILURE);
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(PORT);

    if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        perror("Binding failed");
        close(serverSocket);
        exit(EXIT_FAILURE);
    }

    listen(serverSocket, 5);
    printf("Server listening on port %d...\n", PORT);

    clientSocket = accept(serverSocket, NULL, NULL);
    if (clientSocket < 0) {
        perror("Server accept failed");
        close(serverSocket);
        exit(EXIT_FAILURE);
    }

    while (1) {
        int bytesReceived = recv(clientSocket, buffer, FRAME_SIZE, 0);
        if (bytesReceived <= 0) {
            printf("Connection closed or error occurred\n");
            break;
        }

        int frameNumber = atoi(buffer + 6); 
        printf("Received Frame %d\n", frameNumber);

        random_delay();

        if (is_frame_corrupted()) {
            printf("Frame %d is corrupted. Discarding.\n", frameNumber);
            continue; 
        }

        framesReceived[frameNumber] = 1; 

        sprintf(buffer, "ACK %d", frameNumber);
        send(clientSocket, buffer, strlen(buffer), 0);
        printf("Acknowledgment sent for Frame %d\n", frameNumber);

        int allReceived = 1;
        for (int i = 0; i < MAX_FRAMES; i++) {
            if (framesReceived[i] == 0) {
                allReceived = 0;
                break;
            }
        }
        if (allReceived) {
            printf("All frames received successfully.\n");
            break;
        }
    }

    close(clientSocket);
    close(serverSocket);
    return 0;
}
