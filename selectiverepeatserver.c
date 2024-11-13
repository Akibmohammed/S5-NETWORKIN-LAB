#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdbool.h>

int main()
{
  char buff[1024];
  int serverSocket, clientSocket;
  struct sockaddr_in serverAddr, clientAddr;
  socklen_t addr_size;

  bool receivedFrames[1024] = {false}; // Track received frames

  serverSocket = socket(AF_INET, SOCK_STREAM, 0);
  serverAddr.sin_family = AF_INET;
  serverAddr.sin_port = htons(9999);
  serverAddr.sin_addr.s_addr = INADDR_ANY;

  bind(serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr));
  listen(serverSocket, 5);

  printf("Server listening on port 9999\n");
  addr_size = sizeof(clientAddr);
  clientSocket = accept(serverSocket, (struct sockaddr *)&clientAddr, &addr_size);

  while (1)
  {
    int bytesReceived = recv(clientSocket, buff, sizeof(buff), 0);
    if (bytesReceived <= 0)
      break; // Exit if no data received

    buff[bytesReceived] = '\0';
    int frameNumber;
    sscanf(buff, "Frame %d", &frameNumber);
    printf("Received %s\n", buff);

    // If frame is valid, mark it as received
    if (frameNumber >= 0 && frameNumber < 1024 && !receivedFrames[frameNumber])
    {
      receivedFrames[frameNumber] = true;
    }

    // Send acknowledgment for the received frame
    sprintf(buff, "Ack %d", frameNumber);
    send(clientSocket, buff, strlen(buff), 0);
    printf("Sent acknowledgment for frame %d\n", frameNumber);
  }

  close(clientSocket);
  close(serverSocket);
  return 0;
}
