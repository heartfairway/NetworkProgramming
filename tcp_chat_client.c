#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main(int argc, char *argv[])
{
    int s_fd, len, i;
    char rx_buf[256], tx_buf[256], tc;
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

    if(fcntl(s_fd, F_SETFL, O_NONBLOCK) == -1) {
        printf("Set non-blocking flag error: %d \n", errno);
        exit(-1);
    }

    fcntl(0, F_SETFL, O_NONBLOCK);
    
    i=0;
    printf("You: ");
    fflush(stdout);
    while(1) {
        len=recv(s_fd, rx_buf, 256, 0);
        if(len > 0) {
            rx_buf[len]='\0';
            tx_buf[i]='\0';
            printf("\rReceived: %s        \n", rx_buf);
            printf("You: %s", tx_buf);
            fflush(stdout);
        }
        else {
            if(errno != EAGAIN) {
                printf("Reveiving error: %d / %d \n", len, errno);
                break;
            }
        }

        if(read(0, &tc, 1) > 0) {
            if(tc=='\n') {
                if(tx_buf[0]=='\\') break;

                tx_buf[i]='\0';
                send(s_fd, tx_buf, strlen(tx_buf), 0);
                printf("You: ");
                fflush(stdout);
                i=0;
            }
            else tx_buf[i++]=tc;
        }
    }

    close(s_fd);
    printf("Socket closed.\n");

    return 0;
}
