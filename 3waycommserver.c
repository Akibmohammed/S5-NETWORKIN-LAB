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

void comm(int connfd1, int connfd2) {
    char buff[MAX];
    int i;
    read(connfd1, buff, sizeof(buff));
    for (i = 0; i < strlen(buff); i++) {
        if (buff[i] == 'a' || buff[i] == 'e' || buff[i] == 'i' || buff[i] == 'o' || buff[i] == 'u') {
            buff[i] = '$';
        }
    }
    write(connfd2, buff, sizeof(buff));
}

int main() {
    int sockfd, connfd1, connfd2, ;
    struct sockaddr_in saddr, caddr;
    int len = sizeof(caddr);


    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        printf("Socket creation failed.\n");
        exit(0);
    } else {
        printf("Socket successfully created.\n");
    }

    saddr.sin_family = AF_INET;
    saddr.sin_addr.s_addr = htonl(INADDR_ANY);
    saddr.sin_port = htons(PORT);

    bind(sockfd, (struct sockaddr*)&saddr, sizeof(saddr))
      
    listen(sockfd, 5) 

    connfd1 = accept(sockfd, (struct sockaddr*)&caddr, &len);
    connfd2 = accept(sockfd, (struct sockaddr*)&caddr, &len);

    comm(connfd1, connfd2);
    close(sockfd);
    return 0;
}
