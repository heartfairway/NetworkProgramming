#include <stdio.h>
#include <stdlib.h>
#include <string.h> // memset
#include <unistd.h>
#include <fcntl.h> // fcntl
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h> // inet_ntoa
#include <errno.h>
#include <signal.h>

int run;

void broadcast_message(char *str, int *cfd, int cnt)
{
    int i;

    for(i=0; i<cnt; i++) {
        if(cfd[i] > 0) send(cfd[i], str, strlen(str), 0);
    }
}

void on_signal(int s)
{
    printf("Got signal: %d \n", s);
    run=0;
}

int main(int argc, char **argv)
{
    int socket_fd, tfd, client_fd[32];
    int client_cnt, len, i;
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

    run=1;
    signal(SIGINT, on_signal);
    signal(SIGTERM, on_signal);

    if(listen(socket_fd, 0) == -1) {
        printf("Socket listen error: %d\n", errno);
        exit(-1);
    }
    printf("Start listening.\n");

    sleep(1);
    client_cnt=0;

    while(run) {
        tfd=accept(socket_fd, (struct sockaddr *)&caddr, &caddr_len);

        // detect new user
        if(tfd > 0) {
            fcntl(tfd, F_SETFL, fcntl(tfd, F_GETFL) | O_NONBLOCK);
            client_fd[client_cnt]=tfd;

            sprintf(tx_buf, "Welcome, you are User#%d\n", client_cnt);
            send(tfd, tx_buf, strlen(tx_buf), 0);

            printf("User#%d joined\n", client_cnt);

            sprintf(tx_buf, "### Welcome our new friend, User#%d\n", client_cnt++);
            broadcast_message(tx_buf, client_fd, client_cnt);
        }

        // receive 
        for(i=0; i<client_cnt; i++) {
            len=recv(client_fd[i], rx_buf, 256, 0);

            if(len > 0) {
                if(rx_buf[0]=='\\') {
                    sprintf(tx_buf, "Bye.\n");
                    send(client_fd[i], tx_buf, strlen(tx_buf), 0);

                    close(client_fd[i]);
                    client_fd[i]=-1;
                    sprintf(tx_buf, "\r### User#%d leaves this chatroom.      \n", i, rx_buf);

                    printf("User#%d leaved\n", i);
                }
                else {
                    rx_buf[len]='\0';
                    sprintf(tx_buf, "\r[User#%d] %s      \n", i, rx_buf);
                }

                broadcast_message(tx_buf, client_fd, client_cnt);
            }
        }
    }

    close(socket_fd);

    sprintf(tx_buf, "The chatroom server is going to shutdown.\n");
    broadcast_message(tx_buf, client_fd, client_cnt);

    for(i=0; i<client_cnt; i++) {
        if(client_fd[i] > 0) close(client_fd[i]);
    }

    printf("Socket closed.\n");
    
    return 0;
}
