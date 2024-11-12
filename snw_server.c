#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

int main() {
    int serverSocket, clientSocket;
    struct sockaddr_in serverAddr, clientAddr;
    char buffer[1024];
    int addrLen = sizeof(clientAddr);

    serverSocket = socket(AF_INET, SOCK_STREAM, 0);

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(9999);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    bind(serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr));
    listen(serverSocket, 1);

    printf("Receiver is waiting for frames...\n");

    clientSocket = accept(serverSocket, (struct sockaddr *)&clientAddr, (socklen_t*)&addrLen);

    int frameNumber = 0;
    while (1) {
        // Receive frame from client
        int bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);
        if (bytesReceived <= 0) {
            printf("Client disconnected.\n");
            break;
        }

        buffer[bytesReceived] = '\0';  
        printf("Received: %s\n", buffer);

        // Send acknowledgment back to the client
        sprintf(buffer, "Ack for frame %d", frameNumber);
        send(clientSocket, buffer, strlen(buffer), 0);
        printf("Ack sent for frame %d\n", frameNumber);
        
        frameNumber++;
    }
    close(clientSocket);
    close(serverSocket);
    return 0;
}
