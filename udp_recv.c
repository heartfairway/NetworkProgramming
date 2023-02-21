#include <stdio.h>
#include <stdlib.h>
#include <string.h> // memset
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h> // inet_ntoa
#include <errno.h>

int main(void)
{
    int socket_fd, len;
    char buf[512];
    struct sockaddr_in saddr, caddr;
    unsigned int caddr_len;

    socket_fd=socket(AF_INET, SOCK_DGRAM, 0);
    //socket_fd=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    
    if(socket_fd == -1) {
        printf("Socket creation error: %d\n", errno);
	exit(-1);
    }

    memset((char *)&saddr, 0, sizeof(saddr)); // sizeof(struct sockaddr_in);
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(9001);
    saddr.sin_addr.s_addr = htonl(INADDR_ANY); // 0.0.0.0 = 0

    if(bind(socket_fd, (struct sockaddr *)&saddr, sizeof(saddr)) == -1) {
        printf("Socket binding error: %d\n", errno);
	exit(-1);
    }

    len = recvfrom(socket_fd, buf, 512, 0, (struct sockaddr *)&caddr, &caddr_len);
    buf[len] = '\0';

    if(len == -1) {
        printf("Receiving error: %d\n", errno);
    }
    else {
        printf("Reveived %d bytes \n", len);
	printf("Source: %s:%d \n", inet_ntoa(caddr.sin_addr), ntohs(caddr.sin_port));
	printf("Message: %s \n", buf);
    }

    close(socket_fd);
    
    return 0;
}
