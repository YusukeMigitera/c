/* This function receives socket file discriptor and pointer to text terminated by null of sending object.
 * It guarantees to send all bytes of string.
 * If it successes, returns 1.
 */
int send_string(int sockfd, unsigned char *buffer) {
    int sent_bytes, bytes_to_send;
    bytes_to_send = strlen(buffer);
    while(bytes_to_send > 0) { // send()は第三引数のバイトを送信できるとは限らない
        sent_bytes = send(sockfd, buffer, bytes_to_send, 0);
        if(sent_bytes == -1) return 0;
        bytes_to_send -= sent_bytes;
        buffer += sent_bytes;
    }
    return 1;
}

/* This function
 * It receives data from socket
 */
int recv_line(int sockfd, unsigned char *dest_buffer) {
#define EOL "\r\n" // EOLバイト
#define EOL_SIZE 2
    unsigned char *ptr;
    int eol_matched = 0;

    ptr = dest_buffer;
    while(recv(sockfd, ptr, 1, 0) == 1) { // 1バイトずつ読む
        if(*ptr == EOL[eol_matched]) { // 行末記号か
            eol_matched++;
            if(eol_matched == EOL_SIZE) {
                *(ptr + 1 - EOL_SIZE) = '\0'; // 文字列を終端させる
                return strlen(dest_buffer);
            }
        } else {
            eol_matched = 0;
        }
        ptr++; // 
    }
    return 0; EOL文字が見つからなかった。
}