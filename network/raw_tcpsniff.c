#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h> // ソケット関数 bits/socket.hを自動的にインクルード
#include <netinet/in.h> // ソケットアドレス構造体sockaddr_in
#include <arpa/inet.h>  // バイト順

#include "../functions/hacking.h"

int main(void) {
    int i, recv_length, sockfd;
    u_char buffer[9000];

    if((sockfd = socket(PF_INET, SOCK_RAW, IPPROTO_TCP)) == -1)
        fatal("Failed to create socket.");

    for(i=0; i<3; i++) {
        recv_length = recv(sockfd, buffer, 8000, 0);
        printf("got %d bytes of packet.\n", recv_length);
        dump(buffer, recv_length);
    }
}