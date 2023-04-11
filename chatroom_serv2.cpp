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
        int is_setup(void);
        int send_message(char *str);
        int receive_message(char *buf, int buf_len);
        void logout(void);
};

class Chatroom
{
    private:
        char buf[256];
        int listen_sock;
        int client_cnt;
        UserSession client[32];
        int (*forward_prework)(char *buf, int client_id);
    public:
        int init(int port);
        void poll_routine(void);
        void broadcast_message(char *str);
        void forward_prework_reg(int (*proc)(char *buf, int client_id));
        void uninit(void);
};

// UserSession member functions

int UserSession::setup(int fd)
{
    sock=fd;
    return 0;
}

int UserSession::is_setup(void)
{
    if(sock>0) return 1;
    else return 0;
}

int UserSession::send_message(char *str)
{
    return send(sock, str, strlen(str), 0);
}

int UserSession::receive_message(char *buf, int buf_len)
{
    return recv(sock, buf, buf_len, 0);
}

void UserSession::logout(void)
{
    close(sock);
    sock=-1;
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

    if(fcntl(listen_sock, F_SETFL, fcntl(listen_sock, F_GETFL) | O_NONBLOCK) == -1) {
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
    int i, len, tfd;
    struct sockaddr_in caddr;
    unsigned int caddr_len;

    // detect new user
    tfd=accept(listen_sock, (struct sockaddr *)&caddr, &caddr_len);
        
    if(tfd > 0) {
        fcntl(tfd, F_SETFL, fcntl(tfd, F_GETFL) | O_NONBLOCK);
        client[client_cnt].setup(tfd);

        sprintf(buf, "Welcome, you are User#%d\n", client_cnt);
        client[client_cnt].send_message(buf);

        sprintf(buf, "\r### Welcome our new friend, User#%d      \n", client_cnt);
        broadcast_message(buf);

        std::cout<<"User#"<<client_cnt<<" joined."<<std::endl;

        client_cnt++;
    }

    // receive
    for(i=0; i<client_cnt; i++) {
        if(client[i].is_setup()==0) continue;

        len=client[i].receive_message(buf, 256);

        if(len > 0) {
            if(buf[0]=='\\') {
                client[i].send_message("Bye.\n");

                client[i].logout();

                sprintf(buf, "\r### User#%d leaves this chatroom.      \n", i);
                broadcast_message(buf);

                std::cout<<"User#"<<i<<" leaved"<<std::endl;
            }
            else {
                buf[len]='\0';
                client[i].send_message("\33[1A");

                (*forward_prework)(buf, i);
                broadcast_message(buf);
            }
        }
    }
}

void Chatroom::broadcast_message(char *str)
{
    int i;

    for(i=0; i<client_cnt; i++) {
        if(client[i].is_setup()==0) continue;

        client[i].send_message(str);
    }
}

void Chatroom::forward_prework_reg(int (*proc)(char *buf, int client_id))
{
    forward_prework=proc;
}

void Chatroom::uninit(void)
{
    int i;

    close(listen_sock);

    for(i=0; i<client_cnt; i++) {
        if(client[i].is_setup()==0) continue;

        client[i].logout();
    }
}

void ignore_CRLF(char *c)
{
    for(; *c!='\0'; c++) if(*c=='\r' || *c=='\n') *c=' ';
}

int forward_tagging(char *buf, int client_id)
{
    char tbuf[256];

    ignore_CRLF(buf);

    sprintf(tbuf, "\33[2K\r[User#%d] %s\n", client_id, buf); // with VT100 control, clean whole line first
    strcpy(buf, tbuf);

    return strlen(tbuf);
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

    Chatroom chatroom1;

    if(chatroom1.init(atoi(argv[1]))!=0) {
        std::cout<<"Chatroom initial error"<<std::endl;
        exit(-1);
    }
    else {
        chatroom1.forward_prework_reg(forward_tagging);
        std::cout<<"Chatroom initialized."<<std::endl;
        run=1;
        signal(SIGINT, on_signal);
        signal(SIGTERM, on_signal);
    }

    while(run) {
        chatroom1.poll_routine();
    }

    chatroom1.broadcast_message("The chatroom server is going to shutdown.\n");
    chatroom1.uninit();

    std::cout<<"Chatroom closed."<<std::endl;
    
    return 0;
}
