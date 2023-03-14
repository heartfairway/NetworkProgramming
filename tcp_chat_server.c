#include <stdio.h>
#include <stdlib.h>
#include <string.h> // memset
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h> // inet_ntoa
#include <errno.h>

int main(int argc, char **argv)
{
    int socket_fd, client_fd, len;
    char buf[512];
    struct sockaddr_in saddr, caddr;
    unsigned int caddr_len;

    socket_fd=socket(AF_INET, SOCK_STREAM, 0); // IPPROTO_TCP
    
    if(socket_fd == -1) {
        printf("Socket creation error: %d\n", errno);
	exit(-1);
    }

    memset((char *)&saddr, 0, sizeof(saddr)); // sizeof(struct sockaddr_in);
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(atoi(argv[1]));
    saddr.sin_addr.s_addr = htonl(INADDR_ANY); // 0.0.0.0 = 0

    if(bind(socket_fd, (struct sockaddr *)&saddr, sizeof(saddr)) == -1) {
        printf("Socket binding error: %d\n", errno);
	    exit(-1);
    }

    printf("Socket created and binded.\n");

    if(listen(socket_fd, 0) == -1) {
        printf("Socket listen error: %d\n", errno);
        exit(-1);
    }
    printf("Start listening.\n");

    client_fd=accept(socket_fd, (struct sockaddr *)&caddr, &caddr_len);
    if(client_fd == -1) {
        printf("Connection error: %d\n", errno);
        exit(-1);
    }
    else printf("\nGot a connection from: %s:%d \n", inet_ntoa(caddr.sin_addr), ntohs(caddr.sin_port));

    printf("======\n");

    while(1) {
        len=recv(client_fd, buf, 512, 0);
        buf[len]='\0';
        printf(">>> %s\n", buf);

        printf("<<<");
        scanf("%s", buf);
        if(buf[0]=='\\') break;
        else send(client_fd, buf, strlen(buf), 0);
    }

    close(client_fd);
    close(socket_fd);
    printf("Socket closed.\n");
    
    return 0;
}
