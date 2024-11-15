#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

int main() {
    int serverSocket, clientSocket;
    struct sockaddr_in serverAddr, clientAddr;
    socklen_t addr_size;
    char buffer[1024];
    int num;

    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(9999);
    serverAddr.sin_addr.s_addr = INADDR_ANY;
  
    bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
    listen(serverSocket, 5);

    printf("Server listening on port 9999...\n");

    addr_size = sizeof(clientAddr);
    clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &addr_size);

    recv(clientSocket, buffer, sizeof(buffer), 0);
    num = atoi(buffer); 

    char binaryStr[33];
    sprintf(binaryStr, "%08d", num);  

    send(clientSocket, binaryStr, strlen(binaryStr), 0);

    close(clientSocket);
    close(serverSocket);

    return 0;
}
