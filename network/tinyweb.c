#include <stdio.h>
#include <fcntl.h> // open()
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h> // ファイルステータス（サイズ）
#include <sys/socket.h> // ソケット関数 bits/socket.hを自動的にインクルード
#include <netinet/in.h> // ソケットアドレス構造体sockaddr_in
#include <arpa/inet.h>  // バイト順
#include "../functions/hacking.h"
#include "../functions/hacking-network.h"

#define PORT 80
#define WEBROOT "./webroot" // Webサーバのルートディレクトリ

void handle_connection(int, struct sockaddr_in *); // ウェブリクエストを扱う
int get_file_size(int); // ファイル記述子のサイズを返す

int main(void) {
    int sockfd, new_sockfd, yes=1; // yes
    struct sockaddr_in host_addr, client_addr; // 自らのアドレス, 相手のアドレス
    socklen_t sin_size;

    printf("accept web request from port %d.\n", PORT);
    if((sockfd = socket(PF_INET, SOCK_STREAM, 0)) == -1) fatal("Failed to create socket");
    if(setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) // 使用中と判断されたポートでもバインド
        fatal("set socket SO_REUSEADDR");

    host_addr.sin_family = AF_INET;         // ホストのバイト順 定数AF_INET==PF_INET==2
    host_addr.sin_port = htons(PORT);       // host to network 16bit整数shortをネットワークバイト順に
    host_addr.sin_addr.s_addr = INADDR_ANY; // 自動的に自身のIPを設定
    memset(&(host_addr.sin_zero), '\0', 8); // 構造体の残りをゼロクリア

    if(bind(sockfd, (struct sockaddr *)&host_addr, sizeof(struct sockaddr)) == -1)
        fatal("Failed to bind socket");

    if(listen(sockfd, 5) == -1) // 接続をバックログキューに追加できるようにする。20はキューの最大サイズ
        fatal("Failed to listen socket");

    while(1) { // Acceptループ
        sin_size = sizeof(struct sockaddr_in);
        new_sockfd = accept(sockfd, (struct sockaddr *)&client_addr, &sin_size); // バックログキューから受け付ける
        if(new_sockfd == -1)
            fatal("Failed to receive connection");
        
        handle_connection(new_sockfd, &client_addr);
    }
    return  0;
}

/* クライアントのアドレスからのソケットに対する接続
 * ウェブリクエストとして処理し、ソケット経由で応答、クローズまで
 */
void handle_connection(int sockfd, struct sockaddr_in *client_addr_ptr) {
    unsigned char *ptr, request[500], resource[500];
    int fd, length;

    length = recv_line(sockfd, request);
    printf("received request from %s:%d \"%s\".\n",
        inet_ntoa(client_addr_ptr->sin_addr),ntohs(client_addr_ptr->sin_port),  request);
    
    ptr = strstr(request, "HTTP/"); // 有効に見えるリクエストを検索する
    if(ptr == NULL) printf("not HTTP!\n");
    else {
        *ptr = 0; // URLの末尾でバッファを終端させる
        ptr = NULL; // 無効なリクエストのフラグとして使用
        if(strncmp(request, "GET ", 4) == 0) ptr = request+4;
        if(strncmp(request, "HEAD ", 5) == 0) ptr = request+5;

        if(ptr == NULL) printf("\tUnknown request!\n");
        else {
            ptr[strlen(ptr) - 1] = '\0';
            if(ptr[strlen(ptr) - 1] == '/') // '/'で終わるリソースの場合
                strcat(ptr, "index.html");  // 末尾に'index.html'を追加
            strcpy(resource, WEBROOT);
            strcat(resource, ptr);

            fd = open(resource, O_RDONLY, 0);
            printf("\topen \'%s\'", resource); //here
            if(fd == -1) { // ファイルが見つからない場合
                printf(" 404 Not Found\n");
                send_string(sockfd, "HTTP/1.0 404 NOT FOUND\r\n");
                send_string(sockfd, "Server: Tiny webserver\r\n\r\n");
                send_string(sockfd, "<html><head><title>404 Not Found</title></head>");
                send_string(sockfd, "<body><h1>URL not found</h1></body></html>\r\n");
            } else {
                printf(" 200 OK\n");
                send_string(sockfd, "HTTP/1.0 200 OK\r\n");
                send_string(sockfd, "Server: Tiny webserver\r\n\r\n");
                if(ptr == request+4) { // GET request
                    if((length = get_file_size(fd)) == -1) fatal("Failed to get size of resource file.");
                    if((ptr = (unsigned char *) malloc(length)) == NULL) fatal("Failed to allocate memory when read resource.");

                    read(fd, ptr, length); // ファイルをメモリ内に読み込む
                    send(sockfd, ptr, length, 0); // ソケットに送信
                    free(ptr); // メモリ開放
                }
                close(fd); // ファイルをクローズ
            }
        }
    }
    shutdown(sockfd, SHUT_RDWR); // ソケットをクローズ
}

// オープンされているファイル記述子を受け取り、サイズを返す
int get_file_size(int fd) {
    struct stat stat_struct;
    if(fstat(fd, &stat_struct) == -1) return -1;
    return (int) stat_struct.st_size;
}