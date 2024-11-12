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
    int clientsocket;
    Struct sockaddr_in saddr,caddr;
    char buffer[1024];
    int len=sizeof(saddr);

    clientsocket=socket(AF_INET,SOCK_STREAM,0);

    saddr.sin_family=AF_INET;
    saddr.sin_addr.s_addr=htonl("127.0.0.1");
    saddr.sin_port=htons(PORT);
    
   connect(clientsocket,(Struct sockaddr*)&saddr,len);
    
    while(1){
        printf("Enter message to client: ");
        fgets(buffer,sizeof(buffer),stdin);
        send(clientsocket,buffer,1024,0);

        int byterecieved=recvfrom(clientsocket,buffer,1024,0,(Struct sockaddr*)&saddr,&len);
        if(byterecieved<=0){
            break;
        }
        buffer[byterecieved]='\0';
        printf("client: %s",buffer);

        
    }
    close(clientsocket);
    return 0;
}
