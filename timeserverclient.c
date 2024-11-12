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
    struct sockaddr_in saddr;
    char buffer[1024];
    bzero(buffer,1024);
    int len=sizeof(saddr)

    sockfd=socket(AF_INET,SOCK_DGRAM,0);
    if(sockfd==-1){
        printf("socket not created");
        exit(0);
    }
    printf("socket created successfully");

    saddr.sin_family=AF_INET;
    saddr.sin_addr.s_addr=htonl(INADDR_ANY);
    saddr.sin_port=htons(PORT);

    strcpy(buffer,"what is the time now?");
    printf("client: %s",data);
    sendto(sockfd,buffer,1024,0,(struct sockaddr*)&saddr,sizeof(saddr));
    int byterecieved=recvfrom(sockfd,buffer,1024,0,(struct sockaddr*)&saddr,&len);
    data[byterecieved]='\0';
    printf("time from seveer is: %s",data);

