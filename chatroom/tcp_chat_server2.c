#include <stdio.h>
#include <stdlib.h>
#include <string.h> // memset
#include <unistd.h>
#include <fcntl.h> // fcntl
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h> // inet_ntoa
#include <errno.h>

int main(int argc, char **argv)
{
    int socket_fd, client_fd[16], numClient, len, i, clientIdx;
    char rx_buf[256], tx_buf[256], tc;
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

    if(fcntl(socket_fd, F_SETFL, fcntl(socket_fd, F_GETFL) | O_NONBLOCK) == -1) {
        printf("Set non-blocking flag error: %d \n", errno);
        exit(-1);
    }

    if(listen(socket_fd, 0) == -1) {
        printf("Socket listen error: %d\n", errno);
        exit(-1);
    }
    printf("Start listening.\n");
    numClient=0;

    /*client_fd=accept(socket_fd, (struct sockaddr *)&caddr, &caddr_len);
    if(client_fd == -1) {
        printf("Connection error: %d\n", errno);
        exit(-1);
    }
    else printf("\nGot a connection from: %s:%d \n", inet_ntoa(caddr.sin_addr), ntohs(caddr.sin_port));

    if(fcntl(client_fd, F_SETFL, fcntl(client_fd, F_GETFL) | O_NONBLOCK) == -1) {
        printf("Set non-blocking flag error: %d \n", errno);
        exit(-1);
    }*/

    printf("======\n");

    fcntl(0, F_SETFL, O_NONBLOCK);

    i=0;
    printf("You: ");
    fflush(stdout);
    while(1) {
        client_fd[numClient]=accept(socket_fd, (struct sockaddr *)&caddr, &caddr_len);
        if(client_fd[numClient] == -1) {
            if(errno!=EAGAIN) {
                printf("Connection error: %d\n", errno);
                exit(-1);
            }
        }
        else {
            printf("\nGot a connection from: %s:%d \n", inet_ntoa(caddr.sin_addr), ntohs(caddr.sin_port));
            if(fcntl(client_fd[numClient] , F_SETFL, fcntl(client_fd[numClient], F_GETFL) | O_NONBLOCK) == -1) {
                printf("Set non-blocking flag error: %d \n", errno);
                exit(-1);
            }
            numClient++;
        }

        for(clientIdx=0; clientIdx<numClient; clientIdx++) {
            len=recv(client_fd[clientIdx], rx_buf, 256, 0);
            if(len > 0) {
                rx_buf[len]='\0';
                fflush(stdin);
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
        }

        if(read(0, &tc, 1) > 0) {
            if(tc=='\n') {
                if(tx_buf[0]=='\\') break;

                tx_buf[i]='\0';
                for(clientIdx=0; clientIdx<numClient; clientIdx++) {
                    send(client_fd[clientIdx], tx_buf, strlen(tx_buf), 0);
                }
                printf("You: ");
                fflush(stdout);
                i=0;
            }
            else tx_buf[i++]=tc;
        }
    }

    close(client_fd[0]);
    close(socket_fd);
    printf("Socket closed.\n");
    
    return 0;
}
