#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

int main() {
    int clientSocket;
    struct sockaddr_in serverAddr;
    char buffer[1024];
    int num;

    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(9999);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
  
    connect(clientSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr));

    printf("Enter a decimal number: ");
    scanf("%d", &num);

    sprintf(buffer, "%d", num); 
    send(clientSocket, buffer, strlen(buffer), 0);

    recv(clientSocket, buffer, sizeof(buffer), 0);
    printf("Received binary number: %s\n", buffer);

    close(clientSocket);

    return 0;
}
