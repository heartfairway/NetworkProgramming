#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main(void)
{
    int s_fd, len;
    char buf[512];
    struct sockaddr_in server_addr;

    s_fd = socket(AF_INET, SOCK_STREAM, 0);

    if(s_fd < 0){
        printf("Socket fd open error: %d \n", errno);
	exit(-1);
    }

    memset((char *) &server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(9001);
    inet_aton("127.0.0.1", &server_addr.sin_addr);

    if(connect(s_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        printf("Connection error: %d \n", errno);
        exit(-1);
    }

    sprintf(buf, "Hello from client!");
    if(send(s_fd, buf, strlen(buf), 0) == -1) {
        printf("Sending error: %d \n", errno);
        exit(-1);
    }
    printf("Message sent.\n");

    len=recv(s_fd, buf, 512, 0);
    if(len == -1) {
        printf("Receiving error: %d \n", errno);
        exit(-1);
    }

    buf[len]='\0';
    printf("Message from server: %s\n", buf);

    close(s_fd);

    return 0;
}
