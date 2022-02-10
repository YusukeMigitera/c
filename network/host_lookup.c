// 入力されたホスト名からIPアドレスを出力する
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h> // ソケット関数 bits/socket.hを自動的にインクルード
#include <netinet/in.h> // ソケットアドレス構造体sockaddr_in
#include <arpa/inet.h>  // バイト順

#include <netdb.h>     // DNSを使うための関数、構造体

#include "../functions/hacking.h"

int main(int argc, char *argv[]) {
    struct hostent *host_info;
    struct in_addr *address;

    if(argc < 2) {
        printf("usage: %s <host>\n", argv[0]);
        exit(1);
    }

    host_info = gethostbyname(argv[1]);
    if(host_info == NULL) printf("can't find %s.\n", argv[1]);
    else {
        address = (struct in_addr *) (host_info->h_addr);
        printf("The address of %s is %s.\n", argv[1], inet_ntoa(*address));
    }
}