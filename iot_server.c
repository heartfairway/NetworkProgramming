#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>

void print_sw(uint8_t *sw)
{
    int i;

    printf("\r| ");
    for(i=0; i<8; i++) {
        if(sw[i]) printf("^");
        else printf("_");

        printf(" | ");
    }

    fflush(stdout);
    sleep(0);
}

int main(void)
{
    uint8_t sw[8];
    uint8_t csw, cstat;
    char buf[128];
    int i;
    
    int sfd;
    struct sockaddr_un saddr, caddr;
    unsigned int caddr_len;

    unlink("/run/iot_server");

    sfd=socket(AF_UNIX, SOCK_DGRAM, 0);
    saddr.sun_family=AF_UNIX;
    strcpy(saddr.sun_path, "/run/iot_server");

    bind(sfd, (struct sockaddr *)&saddr, sizeof(saddr));
    chmod("/run/iot_server", 0777);

    for(i=0; i<8; i++) sw[i]=0;

    printf("| 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 |\n");
    print_sw(sw);

    while(recvfrom(sfd, buf, 128, 0, (struct sockaddr *)&caddr, &caddr_len)) {
        csw=buf[0]-48;
        cstat=buf[1]-48;
        sw[csw]=cstat;

        print_sw(sw);
    }

    close(sfd);

    return 0;
}
