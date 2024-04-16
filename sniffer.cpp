#include <linux/if_packet.h>
#include <netinet/in.h>
#include <net/if.h>
#include <net/ethernet.h> 
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <string.h>
#include <stdint.h>
#include <iostream>
#include <iomanip>

int main(int argc, char *argv[])
{
    int socket_fd, len, i;
    struct ifreq ifres;
    struct sockaddr_ll ifaddr;
    void *caddr;
    unsigned int caddr_len;
    uint8_t buf[8192];

    socket_fd=socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));

    memset(&ifres,0,sizeof(struct ifreq));
    strcpy(ifres.ifr_name, argv[1]);

    //get index number of interface
    if(ioctl(socket_fd, SIOCGIFINDEX, &ifres) < 0) {
        std::cout<<"Getting interface index error"<<std::endl;
        return 0;
    }

    std::cout<<"Interface ID:"<<ifres.ifr_ifindex<<std::endl;
 
    ifaddr.sll_family = AF_PACKET;
    ifaddr.sll_protocol = htons(ETH_P_ALL);
    ifaddr.sll_ifindex = ifres.ifr_ifindex;

    if(bind(socket_fd, (struct sockaddr *) &ifaddr, sizeof(struct sockaddr_ll)) < 0) {
        std::cout<<"Binding error"<<std::endl;
        return 0;
    }

    len=recvfrom(socket_fd, buf, 8192, 0, (struct sockaddr *) &caddr, &caddr_len);

    std::cout<<"Received length:"<<len<<std::endl;
    std::cout<<std::hex;
    for(i=0; i<len; i++) {
        std::cout<<std::setfill('0')<<std::setw(2)<<(int)buf[i]<<" ";
    }
    std::cout<<std::dec<<std::endl;

    close(socket_fd);

    return 0;
}
