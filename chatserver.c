#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

int main() {
    int serverSocket, clientSocket;
    char buffer[1024];
    struct sockaddr_in serverAddr;
    int len = sizeof(serverAddr); 
  
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(9999);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    bind(serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr));
    listen(serverSocket, 5);

    printf("Server listening on port 9999...\n");

    clientSocket = accept(serverSocket, (struct sockaddr *)&serverAddr, &len);

    // Chat loop: receive from client and send back to client
    while (1) {
        int bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);
        if (bytesReceived <= 0) break; // Exit if client disconnects
        buffer[bytesReceived] = '\0';  // Null-terminate the received message
        printf("Client: %s\n", buffer);
        
        printf("Enter message to client: ");
        fgets(buffer, sizeof(buffer), stdin);
        send(clientSocket, buffer, strlen(buffer), 0);
    }
    close(clientSocket);
    close(serverSocket);
    return 0;
}
