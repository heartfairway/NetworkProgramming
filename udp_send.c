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
    int s_fd;
    char buf[512];
    struct sockaddr_in server_addr;

    s_fd = socket(AF_INET, SOCK_DGRAM, 0);

    if(s_fd < 0){
        printf("Socket fd open error: %d \n", errno);
	exit(-1);
    }

    memset((char *) &server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(9001);
    inet_aton("127.0.0.1", &server_addr.sin_addr);

    sprintf(buf, "Hello World!");

    if(-1 == sendto(s_fd, buf, strlen(buf), 0, (struct sockaddr *)&server_addr, sizeof(server_addr))) {
        printf("Transmission error: %d \n", errno);
    }

    close(s_fd);

    return 0;
}
