#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h> // ソケット関数 bits/socket.hを自動的にインクルード
#include <netinet/in.h> // ソケットアドレス構造体sockaddr_in
#include <arpa/inet.h>  // バイト順
#include "../documents/hacking.h"
#include <unistd.h>     // close()

#define PORT 7890

int main(void) {
    int sockfd, new_sockfd; // sockfd上で待ち受ける、new_fdは新たな接続
    struct sockaddr_in host_addr, client_addr; // 自らのアドレス情報
    socklen_t sin_size;
    int recv_length=1, yes=1;
    char buffer[1024];

    if((sockfd = socket(PF_INET, SOCK_STREAM, 0)) == -1) // ソケット生成
    // 定数(protocol family)PF_INET==2, ストリームソケットSOCK_STREAM==1
        fatal("Failed to create socket");

    if(setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) // 使用中と判断されたポートでもバインド
        fatal("set socket SO_REUSEADDR");

    // bind()に構造体host_addrを渡す
    host_addr.sin_family = AF_INET;         // ホストのバイト順 定数AF_INET==PF_INET==2
    host_addr.sin_port = htons(PORT);       // host to network 16bit整数shortをネットワークバイト順に
    host_addr.sin_addr.s_addr = 0;          // 自動的に自身のIPを設定
    memset(&(host_addr.sin_zero), '\0', 8); // 構造体の残りの部分はゼロ

    if(bind(sockfd, (struct sockaddr *)&host_addr, sizeof(struct sockaddr)) == -1)
        fatal("Failed to bind socket");

    if(listen(sockfd, 5) == -1) // 接続をバックログキューに追加できるようにする。5はキューの最大サイズ
        fatal("Failed to listen socket");

    while(1) { // Acceptループ
        sin_size = sizeof(struct sockaddr_in);
        new_sockfd = accept(sockfd, (struct sockaddr *)&client_addr, &sin_size); // バックログキューから受け付ける
        if(new_sockfd == -1)
            fatal("Failed to receive connection");
        printf("server: received connection from port %d of %s.\n", ntohs(client_addr.sin_port), inet_ntoa(client_addr.sin_addr));
        send(new_sockfd, "Hello, world!\n", 13, 0); // 13byte文字列
        recv_length = recv(new_sockfd, &buffer, 1024, 0);
        while(recv_length > 0) {
            printf("Receive: %d bytes received.\n", recv_length);
            dump(buffer, recv_length);
            recv_length = recv(new_sockfd, &buffer, 1024, 0);
        }
        close(new_sockfd);
    }
    return  0;
}