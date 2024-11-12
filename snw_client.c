#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

int main() {
    int clientSocket;
    struct sockaddr_in serverAddr;
    char buffer[1024];
    int numFrames;

    clientSocket = socket(AF_INET, SOCK_STREAM, 0);

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(9999);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    connect(clientSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr));

    printf("Enter the number of frames: ");
    scanf("%d", &numFrames);

    for (int frameNumber = 0; frameNumber < numFrames; frameNumber++) {
        // Send frame to server
        sprintf(buffer, "Frame %d", frameNumber);
        send(clientSocket, buffer, strlen(buffer), 0);
        printf("Sent frame %d\n", frameNumber);

        // Wait for acknowledgment from server
        int bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);
        if (bytesReceived > 0) {
            buffer[bytesReceived] = '\0';
            printf("Ack received: %s\n", buffer);
        }
    }

    printf("End of Stop-and-Wait protocol.\n");

    close(clientSocket);
    return 0;
}
