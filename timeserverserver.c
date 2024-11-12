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

int main(){
    int sockfd;
    struct sockaddr_in saddr,caddr;
    int len=sizeof(caddr);
    char buffer[1024];
    bzero(buffer,1024);

    sockfd=socket(AF_INET,SOCK_DGRAM,0);
    if(sockfd==-1){
        printf("socket not created");
        exit(0);
    }
    printf("socket created successfully");

    saddr.sin_family=AF_INET;
    saddr.sin_addr.s_addr=htonl(INADDR_ANY);
    saddr.sin_port=htons(PORT);
    
    bind(sockfd,(struct sockaddr*)&saddr,sizeof(saddr));

    int byterecieved=recvfrom(sockfd,buffer,1024,0,(struct sockaddr*)&caddr,&len);
    printf("client: %s",buffer);
    bzero(buffer,1024);

    time_t t;
    t=time(NULL);
    strcpy(buffer,ctime(&t));
    printf("time to client: %s",buffer);
    sendto(sockfd,buffer,1024,0,(struct sockaddr*)&caddr,len);
    }
