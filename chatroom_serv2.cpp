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

#include <iostream>

int run;

// class definitions

class UserSession
{
    private:
        int sock;
    public:
        int setup(int fd);
        int send_message(char *str);
        int receive_message(char *buf, int buf_len);
};

class Chatroom
{
    private:
        char buf[256];
        int listen_sock;
        int client_cnt;
        UserSession client[32];
    public:
        int init(int port);
        void poll_routine(void);
        void broadcast_message(char *str);
};

// UserSession member functions

int UserSession::setup(int fd)
{
    sock=-1;
    return 0;
}

int UserSession::send_message(char * str)
{
    return send(sock, str, strlen(str), 0);
}

int UserSession::receive_message(char *buf, int buf_len)
{
    return recv(sock, buf, buf_len, 0);
}

// Chatroom member functions

int Chatroom::init(int port)
{
    int i;
    struct sockaddr_in saddr;

    for(i=0; i<32; i++) client[i].setup(-1);

    listen_sock=socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if(listen_sock == -1) {
        std::cout<<"Socket creation error: "<<errno<<std::endl;
	    return -1;
    }

    memset((char *)&saddr, 0, sizeof(struct sockaddr_in));
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(port);
    saddr.sin_addr.s_addr = htonl(INADDR_ANY);

    if(bind(listen_sock, (struct sockaddr *)&saddr, sizeof(saddr)) == -1) {
        std::cout<<"Socket binding error: "<<errno<<std::endl;
	    return -1;
    }

    if(fcntl(listen_sock, F_SETFL, fcntl(socket_fd, F_GETFL) | O_NONBLOCK) == -1) {
        std::cout<<"Set non-blocking flag error: "<<errno<<std::endl;
        return -1;
    }

    if(listen(listen_sock, 0) == -1) {
        std::cout<<"Socket listen error: "<<errno<<std::endl;
        return -1;
    }

    client_cnt=0;

    return 0;
}

void Chatroom::poll_routine(void)
{
    int i, tfd;
    struct sockaddr_in caddr;

    // detect new user
    tfd=accept(listen_sock, (struct sockaddr *)&caddr, &caddr_len);
        
    if(tfd > 0) {
        fcntl(tfd, F_SETFL, fcntl(tfd, F_GETFL) | O_NONBLOCK);
        client[client_cnt].setup(tfd);

        sprintf(buf, "Welcome, you are User#%d\n", client_cnt);
        client[client_cnt].send(buf);

        sprintf(buf, "\r### Welcome our new friend, User#%d      \n", client_cnt);
        broadcast_message(buf);

        std::cout<<"User#"<<client_cnt<<" joined."<<std::endl;

        client_cnt++;
    }

    // receive
    for(i=0; i<client_cnt; i++) {
        len=client[i].receive_message(buf, 256);

        if(len > 0) {
            if(buf[0]=='\\') {
                client[i].send_message("Bye.\n");

                    close(client_fd[i]);
                    client_fd[i]=-1;

                sprintf(buf, "\r### User#%d leaves this chatroom.      \n", i);
                broadcast_message(buf);

                std::cout<<"User#"<<i<<"leaved"<<std::endl;
            }
            else {
                sprintf(tx_buf, "\33[1A"); // VT100 control, move cursor up 1 line
                send(client_fd[i], tx_buf, strlen(tx_buf), 0);

                rx_buf[len]='\0';
                ignore_CRLF(rx_buf);
                sprintf(tx_buf, "\33[2K\r[User#%d] %s      \n", i, rx_buf); // with VT100 control, clean whole line first
            }

            broadcast_message(tx_buf, client_fd, client_cnt);
        }
    }
}

void broadcast_message(char *str, int *cfd, int cnt)
{
    int i;

    for(i=0; i<cnt; i++) {
        if(cfd[i] > 0) send(cfd[i], str, strlen(str), 0);
    }
}

void ignore_CRLF(char *c)
{
    for(; *c!='\0'; c++) if(*c=='\r' || *c=='\n') *c=' ';
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
    
    unsigned int caddr_len;

    

    printf("Socket created and binded.\n");

    

    run=1;
    signal(SIGINT, on_signal);
    signal(SIGTERM, on_signal);

    
    printf("Start listening.\n");

    sleep(1);
    client_cnt=0;

    while(run) {
        
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
