#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define MAX 80
#define PORT 5004

void comm(int sockfd) {
    char buff[MAX];
    printf("Enter a message: ");
    scanf("%s", buff);
    write(sockfd, buff, sizeof(buff));
}

int main() {
    int sockfd;
    struct sockaddr_in saddr;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd == -1) {
        printf("Socket creation failed.\n");
        exit(0);
    }
    printf("Socket successfully created.\n");

    saddr.sin_family = AF_INET;
    saddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    saddr.sin_port = htons(PORT);

    connect(sockfd, (struct sockaddr*)&saddr, sizeof(saddr))

    comm(sockfd);
    close(sockfd);
    return 0;
}
