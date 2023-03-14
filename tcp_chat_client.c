#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main(int argc, char *argv[])
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
    server_addr.sin_port = htons(atoi(argv[2]));
    inet_aton(argv[1], &server_addr.sin_addr);

    if(connect(s_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        printf("Connection error: %d \n", errno);
        exit(-1);
    }
    else printf("Connected.\n");

    while(1) {
        printf("<<<");
        scanf("%s", buf);
        if(buf[0]=='\\') break;
        else send(s_fd, buf, strlen(buf), 0);

        len=recv(s_fd, buf, 512, 0);
        buf[len]='\0';
        printf(">>> %s\n", buf);
    }

    close(s_fd);
    printf("Socket closed.\n");

    return 0;
}
