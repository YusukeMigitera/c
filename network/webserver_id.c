// HEAD / HTTP/1.0を送りServer:行を抜き出す
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h> // ソケット関数 bits/socket.hを自動的にインクルード
#include <netinet/in.h> // ソケットアドレス構造体sockaddr_in
#include <arpa/inet.h>  // バイト順
#include <netdb.h>     // DNSを使うための関数、構造体

#include "../functions/hacking.h"
#include "../functions/hacking-network.h"

int main(int argc, char *argv[]) {
    int sockfd;
    struct hostent *host_info;
    struct sockaddr_in target_addr;
    unsigned char buffer[4096];

    if(argc < 2) {
        printf("usage: %s <host>\n", argv[0]);
        exit(1);
    }

    if((host_info = gethostbyname(argv[1])) == NULL) fatal("Failed to search host name.");

    if((sockfd = socket(PF_INET, SOCK_STREAM, 0)) == -1) fatal("Failed to create socket.");

    target_addr.sin_family = AF_INET;
    target_addr.sin_port = htons(80);
    target_addr.sin_addr = *((struct in_addr *)host_info->h_addr);
    memset(&(target_addr.sin_zero), '\0', 8); // 構造体の残りの部分をゼロクリア
    if(connect(sockfd, (struct sockaddr *)&target_addr, sizeof(struct sockaddr)) == -1)
        fatal("couldn't connect with the other server");

    send_string(sockfd, "HEAD / HTTP/1.0\r\n\r\n");

    while(recv_line(sockfd, buffer)) {
        if(strncasecmp(buffer, "Server:", 7) == 0) {
            printf("The web server of %s is %s.\n", argv[1], buffer+8);
            exit(0);
        }
    }
    printf("couldn't find server line.");
    exit(1);
}